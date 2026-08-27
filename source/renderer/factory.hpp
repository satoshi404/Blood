#pragma once

#include <renderer/gpu/gpu.hpp>
#include <renderer/backend/backend.hpp>

#include <core/math.hpp>

namespace Factory
{
    INLINE bool init()
    {
        return Gpu::init();
    }

    INLINE void shutdown()
    {
        Gpu::shutdown();
    }

    INLINE void new_frame()
    {
        Gpu::new_frame();
    }

    INLINE const Statistics& statistics()
    {
        return Gpu::statistics();
    }

    INLINE Command make_draw_command(
        DescriptorHandle h,
        const char* label = nullptr)
    {
        return Command::draw(h, label);
    }

    INLINE Command make_clear_command(
        const Color color,
        const char* label = nullptr )
    {
        return Command::clear( color, label );
    }

    INLINE Command make_viewport_command(
        const int_32 viewport_x,
        const int_32 viewport_y,
        const int_32 viewport_w,
        const int_32 viewport_h,
        const char* label = nullptr
    )
    {
        return Command::viewport( viewport_x, viewport_y, viewport_w, viewport_h, label );
    }

    INLINE Command make_swap_command( const char* label = nullptr )
    {
        return Command::swap( label );
    }

    INLINE Command make_transform_command(
        const float_32 x, const float_32 y, const float_32 z,
        const char* label = nullptr)
    {
        Transform transform;
        transform.position.x = x;
        transform.position.y = y;
        transform.position.z = z;
        return Command::transform( transform, label );
    }

    INLINE Command make_material_command(
        const MaterialHandle handle_material,
        const char* label = nullptr)
    {
        return Command::material( nullptr, label);
    }

    INLINE Command make_render_state_command(
        const RenderState& state,
        const char* label = nullptr
    )
    {
        return Command::render_state( state, label );
    }

    INLINE Command make_texture_command(
        TextureHandle handle_texture,
        uint_32 slot,
        const char* label = nullptr)
    {
        return Command::texture( handle_texture, slot, label);
    }

    INLINE void bind_command( Command command )
    {
        Gpu::execute(command);
    }

    INLINE void submit( CommandList& list )
    {
        Gpu::submit(list);
    }

    INLINE Command make_begin_render_pass( const RenderPass& pass, const char* label = nullptr )
    {
        return Command::begin_render_pass( pass, label );
    }

    INLINE Command make_end_render_pass( const char* label = nullptr )
    {
        return Command::end_render_pass( label );
    }

    INLINE Command execute_queue( const RenderQueueHandle& queue, const char* label = nullptr )
    {
        return Command::execute_queue( queue, label );
    }

}
