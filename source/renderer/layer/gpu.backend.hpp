#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>
#include <renderer/gpu/core/gpu.types.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/gpu/resource/gpu.mesh.hpp>
#include <renderer/gpu/resource/gpu.material.hpp>
#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

namespace GpuBackend
{
    bool init();
    void shutdown();

    void clear(const GpuClear &);
    void viewport(const GpuViewport &);
    void swap();

    void set_transform(const GpuTransform &);
    void set_render_state(const GpuRenderState &);

    void bind_shader(GpuShaderHandle shader);
    void bind_texture(GpuTextureHandle texture, u32 slot);

    void push_state();
    void pop_state();

    bool buffer_create(GpuBuffer &, const f32 *data, u32 float_count);
    void buffer_destroy(GpuBuffer &);
    void mesh_draw(const GpuMesh &, GpuPrimitive);
    void material_bind(const GpuMaterial &);

    void draw_cube_2d(const GpuDescriptor &);
    void draw_cube_3d(const GpuDescriptor &);
    void draw_sphere_2d(const GpuDescriptor &);
    void draw_sphere_3d(const GpuDescriptor &);
    void draw_obj_2d(const GpuDescriptor &);
    void draw_obj_3d(const GpuDescriptor &);
}
