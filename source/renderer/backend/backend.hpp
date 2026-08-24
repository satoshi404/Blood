#pragma once

#include <pipeline.hpp>
#include <constants.hpp>

#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/descriptor.hpp>
#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/types.hpp>
#include <renderer/gpu/state/transform.hpp>
#include <renderer/gpu/state/state.hpp>
#include <renderer/gpu/resource/buffer.hpp>
#include <renderer/gpu/resource/mesh.hpp>
#include <renderer/gpu/pool/queue.hpp>

// TODO: New
#include <renderer/gpu/resource/shader.hpp>

#include <renderer/gpu/resource/material.hpp>

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
    extern void bind_texture( TextureHandle texture, uint_32 slot );
    extern void bind_material( const Material& material );

    extern void push_state();
    extern void pop_state();

    extern bool buffer_create( Buffer &buffer, const float_32 *data, uint_32 float_count);
    extern void buffer_destroy( Buffer &buffer );
    extern void mesh_draw( const Mesh &mesh, TopologiePrimitiveType primitive_type );

    extern bool shader_create_from_source(
        Shader& out,
        const char* vertex_src,
        const char* fragment_src
    );
    extern void shader_destroy( Shader& shader );
    extern void bind_shader( const Shader& shader );
    extern void set_shader_color( const Shader& shader, const Color& color );

    extern void draw_obj( const Descriptor &desc );

    extern void draw_cube_2d( const Descriptor &desc );
    extern void draw_cube_3d( const Descriptor &desc );
    extern void draw_sphere_2d( const Descriptor &desc );
    extern void draw_sphere_3d(const Descriptor& descriptor);
}
