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

// TODO: New
#include <renderer/gpu/resource/gpu.shader.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

namespace GpuBackend
{
    extern bool init();
    extern void shutdown();

    extern void clear( const GpuClear &c);
    extern void viewport( const GpuViewport &vp );
    extern void swap();

    extern void set_transform( const GpuTransform &transform );
    extern void set_render_state(const GpuRenderState &state );

    extern void bind_shader( GpuShaderHandle shader );
    extern void bind_texture( GpuTextureHandle texture, u32 slot );

    extern void push_state();
    extern void pop_state();

    extern bool buffer_create( GpuBuffer &buffer, const f32 *data, u32 float_count);
    extern void buffer_destroy( GpuBuffer &buffer );
    extern void mesh_draw( const GpuMesh &mesh, TopologiePrimitive primitive );
    extern void material_bind( const GpuMaterial &material );

    // TODO: New
    extern bool shader_create( GpuShader &shader );
    extern void shader_destroy( GpuShader &shader );
    extern void draw_obj( const GpuDescriptor &desc );

    extern void draw_cube_2d(const GpuDescriptor &desc );
    extern void draw_cube_3d(const GpuDescriptor &desc );
    extern void draw_sphere_2d(const GpuDescriptor &desc );
    extern void draw_sphere_3d(const GpuDescriptor &desc );
}
