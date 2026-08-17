#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/core/gpu.types.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/resource/gpu.mesh.hpp>
#include <renderer/gpu/resource/gpu.material.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>

struct GpuDescriptor
{
    GpuDrawType type = GpuDrawType_Cube;
    GpuContext context = GpuContext_2D; // Remover ( renderPass apenas sabe o contexto )

    // TODO: Handle Resource
    GpuMeshHandle handle_mesh;
    GpuMaterialHandle handle_material;

   // const GpuMesh *mesh = nullptr;
   // const GpuMaterial *material = nullptr;

    GpuTransform transform = {};
    GpuSize size = {100.0f, 100.0f, 100.0f};

    GpuRenderState render_state = {};
    TopologiePrimitive primitive = TopologiePrimitive_Default;

    i32 layer = 0;
    f32 z_order = 0.0f;
    u32 render_queue = 0;

    f32 bounds_min[3] = {0.0f, 0.0f, 0.0f};
    f32 bounds_max[3] = {0.0f, 0.0f, 0.0f};
    bool bounds_valid = false;

    char label[GpuLimits::LabelSize] = {};
    u64 id = 0;
    u64 timestamp = 0;
    bool dirty = true;
};

struct GpuDescriptorBuilder
{
    GpuDescriptor value = {};

    GpuDescriptorBuilder &type(GpuDrawType v);
    GpuDescriptorBuilder &context(GpuContext v);
    GpuDescriptorBuilder &mesh( GpuMeshHandle h);
    GpuDescriptorBuilder &material(GpuMaterialHandle h );

    GpuDescriptorBuilder &position( const f32 x, const f32 y, const f32 z = 0.0f);
    GpuDescriptorBuilder &rotation( const f32 x, const f32 y, const f32 z = 0.0f);
    GpuDescriptorBuilder &scale(f32 x, f32 y, f32 z = 1.0f);
    GpuDescriptorBuilder &size(f32 w, f32 h, f32 d = 1.0f);

    GpuDescriptorBuilder &primitive( TopologiePrimitive primitive );
    GpuDescriptorBuilder &layer(i32 v);
    GpuDescriptorBuilder &z_order(f32 v);
    GpuDescriptorBuilder &label( const char *label );

    GpuDescriptor build() const;
};
