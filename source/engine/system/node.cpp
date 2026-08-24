#include <engine/system/node.hpp>
#include <engine/system/component.hpp>
#include <engine/system/serializer.hpp>

#include <core/debug.hpp>
#include <core/memory.hpp>
#include <renderer/gpu/state/transform.hpp>

#include <vendor/libc/string.hpp>
#include <vendor/libc/math.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Internal storage

namespace
{
    struct NodeSlot
    {
        Node     node;
        uint_32  generation = 1;
        bool     alive      = false_value;
    };

    List<NodeSlot> g_slots;
    List<uint_32>  g_free_list;
    bool           g_initialized = false_value;
    uint_64        g_alive_count = 0;

    // ------------------------------------------------------------------
    NodeSlot* get_slot( NodeHandle handle )
    {
        if ( !handle.is_valid() )
            return nullptr;

        if ( handle.index >= g_slots.size() )
            return nullptr;

        NodeSlot& slot = g_slots[ handle.index ];
        if ( !slot.alive || slot.generation != handle.generation )
            return nullptr;

        return &slot;
    }

    // Recursive world transform update
    void update_world_recursive( Node* node )
    {
        if ( !node )
            return;

        if ( node->world_dirty || ( node->parent.is_valid() ) )
        {
            TransformOps::update_matrix( node->local );

            if ( node->parent.is_valid() )
            {
                Node* parent = NodeSystem::get( node->parent );
                if ( parent )
                {
                    // Simple composition: parent.world * local
                    // For now we just copy + apply local (full matrix multiply can be added later)
                    node->world = node->local;
                    node->world.position.x += parent->world.position.x;
                    node->world.position.y += parent->world.position.y;
                    node->world.position.z += parent->world.position.z;

                    // TODO: full matrix multiply when matrix math is more complete
                    node->world.matrix = node->local.matrix; // temporary
                }
                else
                {
                    node->world = node->local;
                }
            }
            else
            {
                node->world = node->local;
            }

            node->world_dirty = false_value;
        }

        // Propagate to children
        for ( uint_64 i = 0; i < node->children.size(); ++i )
        {
            Node* child = NodeSystem::get( node->children[ i ] );
            if ( child )
            {
                child->world_dirty = true_value;
                update_world_recursive( child );
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool NodeSystem::init( uint_64 initial_capacity )
{
    if ( g_initialized )
        return true_value;

    g_slots.init( initial_capacity );
    g_free_list.init( 64 );
    g_alive_count = 0;
    g_initialized = true_value;

    Debug::Println( PrintColorType_Green, "[NodeSystem] initialized (capacity %llu)", initial_capacity );
    return true_value;
}

void NodeSystem::shutdown()
{
    if ( !g_initialized )
        return;

    clear_all();

    g_slots.free();
    g_free_list.free();
    g_initialized = false_value;
    g_alive_count = 0;

    Debug::Println( PrintColorType_Cyan, "[NodeSystem] shutdown" );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

NodeHandle NodeSystem::create( const char* name )
{
    if ( !g_initialized )
    {
        Debug::Println( PrintColorType_Red, "[NodeSystem] create called before init" );
        return {};
    }

    uint_32 index = 0;
    uint_32 generation = 1;

    if ( !g_free_list.empty() )
    {
        index = g_free_list.pop();
        NodeSlot& slot = g_slots[ index ];
        generation = slot.generation;
        slot.alive = true_value;
        slot.node = {};
    }
    else
    {
        index = static_cast<uint_32>( g_slots.size() );
        NodeSlot slot = {};
        slot.alive = true_value;
        slot.generation = 1;
        slot.node = {};
        g_slots.append( slot );
    }

    NodeSlot& slot = g_slots[ index ];
    slot.node.handle.index = index;
    slot.node.handle.generation = generation;
    slot.node.set_name( name ? name : "Node" );
    slot.node.children.init( 4 );
    slot.node.components.init( 4 );
    slot.node.local = TransformOps::identity();
    slot.node.world = slot.node.local;
    slot.node.world_dirty = true_value;

    ++g_alive_count;

    return slot.node.handle;
}

bool NodeSystem::destroy( NodeHandle handle )
{
    NodeSlot* slot = get_slot( handle );
    if ( !slot )
        return false_value;

    // Detach from parent
    if ( slot->node.parent.is_valid() )
    {
        remove_child( slot->node.parent, handle );
    }

    // Destroy / detach children
    clear_children( handle );

    // Destroy all components attached to this node
    for ( uint_64 i = 0; i < slot->node.components.size(); ++i )
    {
        ComponentSystem::destroy( slot->node.components[ i ] );
    }
    slot->node.components.free();

    slot->node.children.free();
    slot->alive = false_value;
    slot->generation++; // invalidate existing handles
    if ( slot->generation == 0 )
        slot->generation = 1;

    g_free_list.append( handle.index );
    --g_alive_count;

    return true_value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Node* NodeSystem::get( NodeHandle handle )
{
    NodeSlot* slot = get_slot( handle );
    return slot ? &slot->node : nullptr;
}

const Node* NodeSystem::get_const( NodeHandle handle )
{
    return get( handle );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool NodeSystem::set_parent( NodeHandle child, NodeHandle parent )
{
    Node* child_node = get( child );
    if ( !child_node )
        return false_value;

    // Remove from old parent
    if ( child_node->parent.is_valid() )
    {
        remove_child( child_node->parent, child );
    }

    child_node->parent = parent;
    child_node->mark_world_dirty();

    if ( parent.is_valid() )
    {
        return add_child( parent, child );
    }

    return true_value;
}

bool NodeSystem::add_child( NodeHandle parent, NodeHandle child )
{
    Node* parent_node = get( parent );
    Node* child_node  = get( child );

    if ( !parent_node || !child_node )
        return false_value;

    // Already a child?
    for ( uint_64 i = 0; i < parent_node->children.size(); ++i )
    {
        if ( parent_node->children[ i ] == child )
            return true_value;
    }

    parent_node->children.append( child );
    child_node->parent = parent;
    child_node->mark_world_dirty();

    return true_value;
}

bool NodeSystem::remove_child( NodeHandle parent, NodeHandle child )
{
    Node* parent_node = get( parent );
    if ( !parent_node )
        return false_value;

    for ( uint_64 i = 0; i < parent_node->children.size(); ++i )
    {
        if ( parent_node->children[ i ] == child )
        {
            parent_node->children.remove_at( i );

            Node* child_node = get( child );
            if ( child_node )
            {
                child_node->parent = {};
                child_node->mark_world_dirty();
            }
            return true_value;
        }
    }

    return false_value;
}

void NodeSystem::clear_children( NodeHandle parent )
{
    Node* parent_node = get( parent );
    if ( !parent_node )
        return;

    // We only detach, do not destroy (caller decides)
    for ( uint_64 i = 0; i < parent_node->children.size(); ++i )
    {
        Node* child = get( parent_node->children[ i ] );
        if ( child )
        {
            child->parent = {};
            child->mark_world_dirty();
        }
    }

    parent_node->children.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void NodeSystem::set_local_position( NodeHandle handle, float_32 x, float_32 y, float_32 z )
{
    Node* node = get( handle );
    if ( !node )
        return;

    TransformOps::set_position( node->local, x, y, z );
    node->mark_world_dirty();
}

void NodeSystem::set_local_rotation( NodeHandle handle, float_32 x, float_32 y, float_32 z )
{
    Node* node = get( handle );
    if ( !node )
        return;

    TransformOps::set_rotation( node->local, x, y, z );
    node->mark_world_dirty();
}

void NodeSystem::set_local_scale( NodeHandle handle, float_32 x, float_32 y, float_32 z )
{
    Node* node = get( handle );
    if ( !node )
        return;

    TransformOps::set_scale( node->local, x, y, z );
    node->mark_world_dirty();
}

void NodeSystem::mark_dirty( NodeHandle handle )
{
    Node* node = get( handle );
    if ( node )
        node->mark_world_dirty();
}

void NodeSystem::update_world_transforms( NodeHandle root )
{
    Node* root_node = get( root );
    if ( !root_node )
        return;

    update_world_recursive( root_node );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

uint_64 NodeSystem::count()
{
    return g_alive_count;
}

void NodeSystem::clear_all()
{
    for ( uint_64 i = 0; i < g_slots.size(); ++i )
    {
        if ( g_slots[ i ].alive )
        {
            // Destroy components first
            for ( uint_64 c = 0; c < g_slots[ i ].node.components.size(); ++c )
            {
                ComponentSystem::destroy( g_slots[ i ].node.components[ c ] );
            }
            g_slots[ i ].node.components.free();
            g_slots[ i ].node.children.free();
            g_slots[ i ].alive = false_value;
            g_slots[ i ].generation++;
        }
    }

    g_free_list.clear();
    g_alive_count = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene

bool Scene::init( const char* scene_name )
{
    if ( !NodeSystem::init() )
        return false_value;

    root = NodeSystem::create( scene_name ? scene_name : "Root" );
    if ( !root.is_valid() )
        return false_value;

    if ( scene_name )
    {
        uint_32 i = 0;
        while ( scene_name[i] && i < 63 )
        {
            name[i] = scene_name[i];
            ++i;
        }
        name[i] = '\0';
    }
    else
    {
        name[0] = '\0';
    }

    return true_value;
}

void Scene::free()
{
    if ( root.is_valid() )
    {
        NodeSystem::destroy( root );
        root = {};
    }
    // Note: NodeSystem::shutdown() should be called by Engine
}

NodeHandle Scene::create_node( const char* name )
{
    NodeHandle h = NodeSystem::create( name );
    if ( h.is_valid() && root.is_valid() )
    {
        NodeSystem::add_child( root, h );
    }
    return h;
}

NodeHandle Scene::create_child( NodeHandle parent, const char* name )
{
    NodeHandle h = NodeSystem::create( name );
    if ( h.is_valid() )
    {
        NodeSystem::add_child( parent.is_valid() ? parent : root, h );
    }
    return h;
}

void Scene::update_transforms()
{
    if ( root.is_valid() )
        NodeSystem::update_world_transforms( root );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Scene::save( const char* path ) const
{
    return SceneSerializer::save( *this, path );
}

bool Scene::load( const char* path )
{
    return SceneSerializer::load( *this, path );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////