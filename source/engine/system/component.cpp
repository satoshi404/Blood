#include <engine/system/component.hpp>
#include <engine/system/node.hpp>

#include <core/debug.hpp>
#include <core/memory.hpp>

#include <vendor/libc/string.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace
{
    struct ComponentSlot
    {
        // We store the largest component to keep it simple (union-like)
        // In a more advanced version we can use separate pools per type.
        union
        {
            MeshComponent       mesh;
            MaterialComponent   material;
            RenderComponent     render;
            ScriptComponent     script;
            CameraComponent     camera;
            LightComponent      light;
            Component           base;   // fallback
        } data;

        ComponentType type       = ComponentType_None;
        uint_32       generation = 1;
        bool          alive      = false_value;
    };

    List<ComponentSlot> g_slots;
    List<uint_32>       g_free_list;
    bool                g_initialized = false_value;
    uint_64             g_alive_count = 0;

    // ------------------------------------------------------------------
    ComponentSlot* get_slot( ComponentHandle handle )
    {
        if ( !handle.is_valid() )
            return nullptr;

        if ( handle.index >= g_slots.size() )
            return nullptr;

        ComponentSlot& slot = g_slots[ handle.index ];
        if ( !slot.alive || slot.generation != handle.generation )
            return nullptr;

        return &slot;
    }

    Component* get_base_from_slot( ComponentSlot* slot )
    {
        if ( !slot )
            return nullptr;

        switch ( slot->type )
        {
            case ComponentType_Mesh:     return &slot->data.mesh.base;
            case ComponentType_Material: return &slot->data.material.base;
            case ComponentType_Render:   return &slot->data.render.base;
            case ComponentType_Script:   return &slot->data.script.base;
            case ComponentType_Camera:   return &slot->data.camera.base;
            case ComponentType_Light:    return &slot->data.light.base;
            default:                     return &slot->data.base;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ComponentSystem::init( uint_64 initial_capacity )
{
    if ( g_initialized )
        return true_value;

    g_slots.init( initial_capacity );
    g_free_list.init( 64 );
    g_alive_count = 0;
    g_initialized = true_value;

    Debug::Println( PrintColorType_Green, "[ComponentSystem] initialized (capacity %llu)", initial_capacity );
    return true_value;
}

void ComponentSystem::shutdown()
{
    if ( !g_initialized )
        return;

    clear_all();
    g_slots.free();
    g_free_list.free();
    g_initialized = false_value;
    g_alive_count = 0;

    Debug::Println( PrintColorType_Cyan, "[ComponentSystem] shutdown" );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

ComponentHandle ComponentSystem::create( ComponentType type, NodeHandle owner )
{
    if ( !g_initialized )
    {
        Debug::Println( PrintColorType_Red, "[ComponentSystem] create before init" );
        return {};
    }

    if ( type == ComponentType_None )
        return {};

    uint_32 index = 0;
    uint_32 generation = 1;

    if ( !g_free_list.empty() )
    {
        index = g_free_list.pop();
        ComponentSlot& slot = g_slots[ index ];
        generation = slot.generation;
        slot.alive = true_value;
        Memory::zero( &slot.data, sizeof( slot.data ) );
    }
    else
    {
        index = static_cast<uint_32>( g_slots.size() );
        ComponentSlot slot = {};
        slot.alive = true_value;
        slot.generation = 1;
        g_slots.append( slot );
    }

    ComponentSlot& slot = g_slots[ index ];
    slot.type = type;
    slot.alive = true_value;

    ComponentHandle handle;
    handle.index = index;
    handle.generation = generation;

    // Initialize the concrete component
    Component* base = nullptr;

    switch ( type )
    {
        case ComponentType_Mesh:
            slot.data.mesh = {};
            slot.data.mesh.base.type = type;
            slot.data.mesh.base.handle = handle;
            slot.data.mesh.base.owner = owner;
            slot.data.mesh.base.enabled = true_value;
            slot.data.mesh.base.alive = true_value;
            base = &slot.data.mesh.base;
            break;

        case ComponentType_Material:
            slot.data.material = {};
            slot.data.material.base.type = type;
            slot.data.material.base.handle = handle;
            slot.data.material.base.owner = owner;
            slot.data.material.base.enabled = true_value;
            slot.data.material.base.alive = true_value;
            base = &slot.data.material.base;
            break;

        case ComponentType_Render:
            slot.data.render = {};
            slot.data.render.base.type = type;
            slot.data.render.base.handle = handle;
            slot.data.render.base.owner = owner;
            slot.data.render.base.enabled = true_value;
            slot.data.render.base.alive = true_value;
            base = &slot.data.render.base;
            break;

        case ComponentType_Script:
            slot.data.script = {};
            slot.data.script.base.type = type;
            slot.data.script.base.handle = handle;
            slot.data.script.base.owner = owner;
            slot.data.script.base.enabled = true_value;
            slot.data.script.base.alive = true_value;
            base = &slot.data.script.base;
            break;

        case ComponentType_Camera:
            slot.data.camera = {};
            slot.data.camera.base.type = type;
            slot.data.camera.base.handle = handle;
            slot.data.camera.base.owner = owner;
            slot.data.camera.base.enabled = true_value;
            slot.data.camera.base.alive = true_value;
            slot.data.camera.fov = 60.0f;
            slot.data.camera.near_plane = 0.1f;
            slot.data.camera.far_plane = 1000.0f;
            base = &slot.data.camera.base;
            break;

        case ComponentType_Light:
            slot.data.light = {};
            slot.data.light.base.type = type;
            slot.data.light.base.handle = handle;
            slot.data.light.base.owner = owner;
            slot.data.light.base.enabled = true_value;
            slot.data.light.base.alive = true_value;
            slot.data.light.intensity = 1.0f;
            slot.data.light.range = 10.0f;
            base = &slot.data.light.base;
            break;

        default:
            slot.data.base = {};
            slot.data.base.type = type;
            slot.data.base.handle = handle;
            slot.data.base.owner = owner;
            slot.data.base.enabled = true_value;
            slot.data.base.alive = true_value;
            base = &slot.data.base;
            break;
    }

    // Register on the node
    if ( owner.is_valid() )
    {
        Node* node = NodeSystem::get( owner );
        if ( node )
        {
            // We need a place to store component handles on the Node.
            // For now we use the existing user_data / or we will extend Node.
            // Temporary: we will add a components list to Node in the next edit.
        }
    }

    ++g_alive_count;
    return handle;
}

bool ComponentSystem::destroy( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot )
        return false_value;

    Component* base = get_base_from_slot( slot );
    if ( base && base->type == ComponentType_Script )
    {
        ScriptComponent* script = &slot->data.script;
        if ( script->on_destroy && script->instance )
            script->on_destroy( script->instance );
    }

    // Remove from owner node (will be handled by Node extension)
    if ( base && base->owner.is_valid() )
    {
        // NodeSystem will clean the list when we extend it
    }

    slot->alive = false_value;
    slot->generation++;
    if ( slot->generation == 0 )
        slot->generation = 1;

    Memory::zero( &slot->data, sizeof( slot->data ) );
    slot->type = ComponentType_None;

    g_free_list.append( handle.index );
    --g_alive_count;

    return true_value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Component* ComponentSystem::get_base( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    return get_base_from_slot( slot );
}

const Component* ComponentSystem::get_base_const( ComponentHandle handle )
{
    return get_base( handle );
}

MeshComponent* ComponentSystem::get_mesh( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot || slot->type != ComponentType_Mesh )
        return nullptr;
    return &slot->data.mesh;
}

MaterialComponent* ComponentSystem::get_material( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot || slot->type != ComponentType_Material )
        return nullptr;
    return &slot->data.material;
}

RenderComponent* ComponentSystem::get_render( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot || slot->type != ComponentType_Render )
        return nullptr;
    return &slot->data.render;
}

ScriptComponent* ComponentSystem::get_script( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot || slot->type != ComponentType_Script )
        return nullptr;
    return &slot->data.script;
}

CameraComponent* ComponentSystem::get_camera( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot || slot->type != ComponentType_Camera )
        return nullptr;
    return &slot->data.camera;
}

LightComponent* ComponentSystem::get_light( ComponentHandle handle )
{
    ComponentSlot* slot = get_slot( handle );
    if ( !slot || slot->type != ComponentType_Light )
        return nullptr;
    return &slot->data.light;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

ComponentHandle ComponentSystem::add( NodeHandle node, ComponentType type )
{
    if ( !node.is_valid() )
        return {};

    // Prevent duplicate of same type for now (common pattern)
    ComponentHandle existing = find( node, type );
    if ( existing.is_valid() )
        return existing;

    ComponentHandle h = create( type, node );
    if ( !h.is_valid() )
        return {};

    Node* n = NodeSystem::get( node );
    if ( n )
    {
        n->components.append( h );
    }

    return h;
}

bool ComponentSystem::remove( NodeHandle node, ComponentHandle handle )
{
    if ( !node.is_valid() || !handle.is_valid() )
        return false_value;

    Node* n = NodeSystem::get( node );
    if ( !n )
        return false_value;

    for ( uint_64 i = 0; i < n->components.size(); ++i )
    {
        if ( n->components[ i ] == handle )
        {
            n->components.remove_at( i );
            return destroy( handle );
        }
    }

    return false_value;
}

bool ComponentSystem::remove_by_type( NodeHandle node, ComponentType type )
{
    ComponentHandle h = find( node, type );
    if ( !h.is_valid() )
        return false_value;
    return remove( node, h );
}

ComponentHandle ComponentSystem::find( NodeHandle node, ComponentType type )
{
    Node* n = NodeSystem::get( node );
    if ( !n )
        return {};

    for ( uint_64 i = 0; i < n->components.size(); ++i )
    {
        Component* c = get_base( n->components[ i ] );
        if ( c && c->type == type )
            return n->components[ i ];
    }

    return {};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Typed helpers

MeshComponent* ComponentSystem::add_mesh( NodeHandle node )
{
    ComponentHandle h = add( node, ComponentType_Mesh );
    return get_mesh( h );
}

MaterialComponent* ComponentSystem::add_material( NodeHandle node )
{
    ComponentHandle h = add( node, ComponentType_Material );
    return get_material( h );
}

RenderComponent* ComponentSystem::add_render( NodeHandle node )
{
    ComponentHandle h = add( node, ComponentType_Render );
    return get_render( h );
}

ScriptComponent* ComponentSystem::add_script( NodeHandle node )
{
    ComponentHandle h = add( node, ComponentType_Script );
    return get_script( h );
}

CameraComponent* ComponentSystem::add_camera( NodeHandle node )
{
    ComponentHandle h = add( node, ComponentType_Camera );
    return get_camera( h );
}

LightComponent* ComponentSystem::add_light( NodeHandle node )
{
    ComponentHandle h = add( node, ComponentType_Light );
    return get_light( h );
}

MeshComponent* ComponentSystem::get_mesh_on( NodeHandle node )
{
    return get_mesh( find( node, ComponentType_Mesh ) );
}

RenderComponent* ComponentSystem::get_render_on( NodeHandle node )
{
    return get_render( find( node, ComponentType_Render ) );
}

ScriptComponent* ComponentSystem::get_script_on( NodeHandle node )
{
    return get_script( find( node, ComponentType_Script ) );
}

CameraComponent* ComponentSystem::get_camera_on( NodeHandle node )
{
    return get_camera( find( node, ComponentType_Camera ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void ComponentSystem::update_all( float_64 delta )
{
    for ( uint_64 i = 0; i < g_slots.size(); ++i )
    {
        ComponentSlot& slot = g_slots[ i ];
        if ( !slot.alive || slot.type != ComponentType_Script )
            continue;

        ScriptComponent& script = slot.data.script;
        if ( !script.base.enabled || !script.on_update || !script.instance )
            continue;

        // Only update if owner node is active
        Node* owner = NodeSystem::get( script.base.owner );
        if ( owner && owner->active )
            script.on_update( script.instance, delta );
    }
}

uint_64 ComponentSystem::count()
{
    return g_alive_count;
}

void ComponentSystem::clear_all()
{
    for ( uint_64 i = 0; i < g_slots.size(); ++i )
    {
        if ( g_slots[ i ].alive )
        {
            ComponentHandle h;
            h.index = static_cast<uint_32>( i );
            h.generation = g_slots[ i ].generation;
            destroy( h );
        }
    }

    g_free_list.clear();
    g_alive_count = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////