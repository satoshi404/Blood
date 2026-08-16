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
    GpuContext context = GpuContext_2D;

    // TODO: Handle Resource
    //const GpuMeshHandle handle_mesh;
    //const GpuMaterialHandle handle_material;
    //const GpuMeshHandle handle_mesh;

    const GpuMesh *mesh = nullptr;
    const GpuMaterial *material = nullptr;

    GpuTransform transform = {};
    GpuSize size = {100.0f, 100.0f, 100.0f};

    GpuRenderState render_state = {};
    GpuPrimitive primitive = GpuPrimitive_Default;

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
    GpuDescriptorBuilder &mesh(const GpuMesh *v);
    GpuDescriptorBuilder &material(const GpuMaterial *v);

    GpuDescriptorBuilder &position(f32 x, f32 y, f32 z = 0.0f);
    GpuDescriptorBuilder &rotation(f32 x, f32 y, f32 z = 0.0f);
    GpuDescriptorBuilder &scale(f32 x, f32 y, f32 z = 1.0f);
    GpuDescriptorBuilder &size(f32 w, f32 h, f32 d = 1.0f);

    GpuDescriptorBuilder &primitive(GpuPrimitive v);
    GpuDescriptorBuilder &layer(i32 v);
    GpuDescriptorBuilder &z_order(f32 v);
    GpuDescriptorBuilder &label(const char *v);

    GpuDescriptor build() const;
};
