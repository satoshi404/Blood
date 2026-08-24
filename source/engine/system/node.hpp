#pragma once

#include <core/types.hpp>
#include <core/list.hpp>
#include <core/math.hpp>

#include <renderer/gpu/state/transform.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/core/limits.hpp>

// Forward
struct ComponentTag;
using ComponentHandle = Handle<ComponentTag>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Runtime hooks (kept for compatibility with current runtime system)

IMPORT void _start();
IMPORT void _input();
IMPORT void _update( float_64 delta );
IMPORT void _finish();
IMPORT void _draw();

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node System - hierarchical scene graph
// Clean, low-overhead, fits Blood engine style.

struct NodeTag {};
using NodeHandle = Handle<NodeTag>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Node
{
    static constexpr uint_32 MAX_NAME = 64;

    // Identity
    char             name[ MAX_NAME ] = {};
    NodeHandle       handle           = {};
    bool             active           = true_value;
    bool             visible          = true_value;

    // Hierarchy
    NodeHandle       parent           = {};
    List<NodeHandle> children;

    // Transform (local + cached world)
    Transform        local            = {};
    Transform        world            = {};
    bool             world_dirty      = true_value;

    // Optional visual link (GPU descriptor) - kept for quick access
    DescriptorHandle descriptor       = {};

    // Components attached to this node
    List<ComponentHandle> components;

    // User data / extension point
    void*            user_data        = nullptr;
    uint_64          flags            = 0;

    // ------------------------------------------------------------------
    void mark_world_dirty()
    {
        world_dirty = true_value;
    }

    void set_name( const char* new_name )
    {
        if ( !new_name )
        {
            name[0] = '\0';
            return;
        }

        uint_32 i = 0;
        while ( new_name[i] && i < MAX_NAME - 1 )
        {
            name[i] = new_name[i];
            ++i;
        }
        name[i] = '\0';
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Node Manager

namespace NodeSystem
{
    bool init( uint_64 initial_capacity = 256 );
    void shutdown();

    // Create / Destroy
    NodeHandle create( const char* name = "Node" );
    bool       destroy( NodeHandle handle );

    // Access
    Node*       get( NodeHandle handle );
    const Node* get_const( NodeHandle handle );

    // Hierarchy
    bool set_parent( NodeHandle child, NodeHandle parent );
    bool add_child( NodeHandle parent, NodeHandle child );
    bool remove_child( NodeHandle parent, NodeHandle child );
    void clear_children( NodeHandle parent );

    // Transform helpers
    void set_local_position( NodeHandle handle, float_32 x, float_32 y, float_32 z );
    void set_local_rotation( NodeHandle handle, float_32 x, float_32 y, float_32 z );
    void set_local_scale( NodeHandle handle, float_32 x, float_32 y, float_32 z );

    void mark_dirty( NodeHandle handle );
    void update_world_transforms( NodeHandle root ); // recursive from root

    // Utility
    uint_64 count();
    void    clear_all();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene - root container for a hierarchy of nodes

struct Scene
{
    NodeHandle root = {};
    char       name[ 64 ] = {};

    bool init( const char* scene_name = "MainScene" );
    void free();

    NodeHandle create_node( const char* name = "Node" );
    NodeHandle create_child( NodeHandle parent, const char* name = "Child" );

    void update_transforms();

    // Serialization (.scene text format)
    bool save( const char* path ) const;
    bool load( const char* path );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////