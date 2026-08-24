#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/core/types.hpp>
#include <renderer/gpu/state/transform.hpp>
#include <renderer/gpu/state/state.hpp>
#include <renderer/gpu/resource/mesh.hpp>
#include <renderer/gpu/core/limits.hpp>

struct Descriptor
{
    DrawType draw_type = DrawType_Cube;
    ContextType context_type = ContextType_3D; // TODO: REMOVE

    // TODO: Handle Resource
    MeshHandle handle_mesh;
    MaterialHandle handle_material;


    Transform transform = {};
    GpuSize size = {100.0f, 100.0f, 100.0f};

    RenderState render_state = {};
    TopologiePrimitiveType primitive_type = TopologiePrimitiveType_Default;

    int_32 layer = 0;
    float_32 z_order = 0.0f;
    uint_32 render_queue = 0;

    float_32 bounds_min[3] = {0.0f, 0.0f, 0.0f};
    float_32 bounds_max[3] = {0.0f, 0.0f, 0.0f};
    bool bounds_valid = false;

    char label[ Limits::Label_Size ] = {};
    uint_64 id = 0;
    uint_64 timestamp = 0;
    bool dirty = true;
};

struct DescriptorBuilder
{
    Descriptor value = {};

    DescriptorBuilder &type( DrawType draw_type );
    DescriptorBuilder &context( ContextType context_type );
    DescriptorBuilder &mesh( MeshHandle handle );
    DescriptorBuilder &material( MaterialHandle handle );

    DescriptorBuilder &position( const float_32 x, const float_32 y, const float_32 z = 0.f);
    DescriptorBuilder &rotation( const float_32 x, const float_32 y, const float_32 z = 0.f);
    DescriptorBuilder &scale( const float_32 x, const float_32 y, const float_32 z = 1.f);
    DescriptorBuilder &size( const float_32 w, const float_32 h, const float_32 d = 1.f);

    DescriptorBuilder &primitive( TopologiePrimitiveType primitive_type );
    DescriptorBuilder &layer( const int_32 layer );
    DescriptorBuilder &z_order( const float_32 value_order );
    DescriptorBuilder &label( const char *label );

    Descriptor build() const;
};
