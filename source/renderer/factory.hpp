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

    INLINE DescriptorHandle create_descriptor(const Descriptor& descriptor )
    {
        return Gpu::create_descriptor( descriptor );
    }

    INLINE bool update_descriptor( const DescriptorHandle handle, const Descriptor& descriptor )
    {
        return Gpu::update_descriptor( handle, descriptor );
    }

    INLINE bool destroy_descriptor( const DescriptorHandle handle )
    {
        return Gpu::destroy_descriptor( handle );
    }

    INLINE const Descriptor* get_descriptor( const DescriptorHandle handle )
    {
        return Gpu::get_descriptor( handle );
    }

    INLINE Descriptor* get_descriptor_mutable( const DescriptorHandle handle )
    {
        return Gpu::get_descriptor_mutable( handle );
    }

    INLINE RenderQueueHandle create_render_queue( const char* label = nullptr )
    {
        return RenderQueuePool::create( label );
    }
    INLINE bool destroy_render_queue( const RenderQueueHandle handle )
    {
        return RenderQueuePool::destroy( handle );
    }

    INLINE RenderQueue* get_render_queue( const RenderQueueHandle handle )
    {
        return RenderQueuePool::get( handle );
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

    INLINE RenderPass make_simple_pass(
        const int_32 x, const int_32 y, const int_32 width, const int_32 height,
        RenderQueueHandle queue,
        ContextType context_type,
        Color color,
        const char* label = nullptr
    )
    {
        RenderPass pass;
        pass.viewport = { x, y, width, height };
        pass.queue = queue;
        pass.color.load_op = LoadOpType_Clear;

        Color normal_color = color.normalized();

        const float_32 r = normal_color.r;
        const float_32 g = normal_color.g;
        const float_32 b = normal_color.b;
        const float_32 a = normal_color.a;

        pass.color.clear = { r, g, b, a };
        pass.clear_enabled = true_value;
        pass.context_type = context_type;

        if ( label )
        {
            strncpy( pass.label, label, Limits::Label_Size - 1 );
            pass.label[ Limits::Label_Size - 1] = '\0';
        }

        return pass;
    }

}
