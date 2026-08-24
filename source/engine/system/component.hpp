#pragma once

#include <core/types.hpp>
#include <core/list.hpp>
#include <renderer/gpu/core/handles.hpp>

// Forward declare Node types to avoid circular include
struct NodeTag;
using NodeHandle = Handle<NodeTag>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component System
// Attached to Nodes. Low-overhead, type-id based, pool allocated.
// Fits Blood engine style (handles + generation, no heavy RTTI).

struct ComponentTag {};
using ComponentHandle = Handle<ComponentTag>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component Types

enum_type( ComponentType, uint_32 )
{
    ComponentType_None      = 0,
    ComponentType_Mesh      = 1,
    ComponentType_Material  = 2,
    ComponentType_Render    = 3,   // links to Descriptor + draw settings
    ComponentType_Script    = 4,
    ComponentType_Camera    = 5,
    ComponentType_Light     = 6,
    ComponentType_Custom    = 100, // user extension start
    ComponentType_Count
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Base Component data (common header)

struct Component
{
    ComponentType   type        = ComponentType_None;
    ComponentHandle handle      = {};
    NodeHandle      owner       = {};
    bool            enabled     = true_value;
    bool            alive       = false_value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Concrete Components

struct MeshComponent
{
    Component       base        = {};
    MeshHandle      mesh        = {};
    // future: lod, etc.
};

struct MaterialComponent
{
    Component       base        = {};
    MaterialHandle  material    = {};
};

struct RenderComponent
{
    Component           base            = {};
    DescriptorHandle    descriptor      = {};
    int_32              layer           = 0;
    float_32            z_order         = 0.0f;
    bool                cast_shadow     = false_value;
    bool                receive_shadow  = true_value;
};

struct ScriptComponent
{
    Component       base        = {};
    void*           instance    = nullptr;   // user script object
    // function pointers for update/draw if needed later
    void          (*on_update)( void* instance, float_64 delta ) = nullptr;
    void          (*on_start)( void* instance ) = nullptr;
    void          (*on_destroy)( void* instance ) = nullptr;
};

struct CameraComponent
{
    Component       base        = {};
    float_32        fov         = 60.0f;
    float_32        near_plane  = 0.1f;
    float_32        far_plane   = 1000.0f;
    bool            is_orthographic = false_value;
    float_32        ortho_size  = 10.0f;
};

struct LightComponent
{
    Component       base        = {};
    // Color + intensity later
    float_32        intensity   = 1.0f;
    float_32        range       = 10.0f;
    uint_32         light_type  = 0; // 0 = point, 1 = directional, etc.
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component System API

namespace ComponentSystem
{
    bool init( uint_64 initial_capacity = 512 );
    void shutdown();

    // Generic create (returns handle, type is set internally)
    ComponentHandle create( ComponentType type, NodeHandle owner );
    bool            destroy( ComponentHandle handle );

    // Access
    Component*       get_base( ComponentHandle handle );
    const Component* get_base_const( ComponentHandle handle );

    // Typed getters (return nullptr if wrong type or invalid)
    MeshComponent*      get_mesh( ComponentHandle handle );
    MaterialComponent*  get_material( ComponentHandle handle );
    RenderComponent*    get_render( ComponentHandle handle );
    ScriptComponent*    get_script( ComponentHandle handle );
    CameraComponent*    get_camera( ComponentHandle handle );
    LightComponent*     get_light( ComponentHandle handle );

    // Convenience on Node
    ComponentHandle add( NodeHandle node, ComponentType type );
    bool            remove( NodeHandle node, ComponentHandle handle );
    bool            remove_by_type( NodeHandle node, ComponentType type );
    ComponentHandle find( NodeHandle node, ComponentType type );

    // Typed convenience
    MeshComponent*     add_mesh( NodeHandle node );
    MaterialComponent* add_material( NodeHandle node );
    RenderComponent*   add_render( NodeHandle node );
    ScriptComponent*   add_script( NodeHandle node );
    CameraComponent*   add_camera( NodeHandle node );
    LightComponent*    add_light( NodeHandle node );

    MeshComponent*     get_mesh_on( NodeHandle node );
    RenderComponent*   get_render_on( NodeHandle node );
    ScriptComponent*   get_script_on( NodeHandle node );
    CameraComponent*   get_camera_on( NodeHandle node );

    // Update (calls script on_update etc.)
    void update_all( float_64 delta );

    uint_64 count();
    void    clear_all();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////