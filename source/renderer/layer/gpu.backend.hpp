#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/types.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/gpu/resource/gpu.mesh.hpp>
#include <renderer/gpu/resource/gpu.material.hpp>
#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/pool/gpu.render.queue.pool.hpp>

// TODO: New
#include <renderer/gpu/resource/gpu.shader.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

namespace Backend
{
    extern bool init();
    extern void shutdown();

    extern void clear( const Color clear_color );
    extern void viewport( const Viewport &viewport );
    extern void swap();

    extern void set_transform( const Transform &transform );
    extern void set_render_state( const RenderState &state );

    extern void bind_shader( ShaderHandle shader );
    extern void bind_texture( TextureHandle texture, u32 slot );

    extern void push_state();
    extern void pop_state();

    extern bool buffer_create( Buffer &buffer, const f32 *data, u32 float_count);
    extern void buffer_destroy( Buffer &buffer );
    extern void mesh_draw( const Mesh &mesh, TopologiePrimitiveType primitive_type );
    extern void material_bind( const Material &material );

    extern bool shader_create( Shader &shader );
    extern void shader_destroy( Shader &shader );
    extern void draw_obj( const Descriptor &desc );

    extern void draw_cube_2d( const Descriptor &desc );
    extern void draw_cube_3d( const Descriptor &desc );
    extern void draw_sphere_2d( const Descriptor &desc );
    extern void draw_sphere_3d( const Descriptor &desc );
}
