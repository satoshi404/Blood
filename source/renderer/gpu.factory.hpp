#pragma once

#include <renderer/gpu/gpu.hpp>
#include <renderer/layer/gpu.backend.hpp>

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

    INLINE const Statistics& statistics() const
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
        return GpuRenderQueuePool::get( handle );
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
        return Command::clear( color.r, color.g, color.b, color.a, label );
    }

    INLINE Command make_viewport_command(
        const i32 viewport_x,
        const i32 viewport_y,
        const i32 viewport_w,
        const i32 viewport_h,
        const char* label = nullptr
    )
    {
        return Command::viewport( x, y, width, height, label );
    }

    INLINE Command make_swap_command( const char* label = nullptr )
    {
        return Command::swap( label );
    }

    INLINE Command make_transform_command(
        const f32 x, const f32 y, const f32 z,
        const char* label = nullptr)
    {
        Transform t = {};
        t.position[0] = x;
        t.position[1] = y;
        t.position[2] = z;
        return Command::transform( t, label );
    }

    INLINE Command make_material_command(
        const MaterialHandle handle_material,
        const char* label = nullptr)
    {
        return GpuCommand::material( nullptr, label);
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
        u32 slot,
        const char* label = nullptr)
    {
        return Command::texture(texture, slot, label);
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
        const i32 x, const i32 y, const i32 width, const i32 height,
        RenderQueueHandle queue,
        Context context,
        const Color color,
        const char* label = nullptr
    )
    {
        RenderPass pass;
        pass.viewport = { x, y, width, height };
        pass.queue = queue;
        pass.color.load_op = LoadOpType_Clear;

        const float r = fClamp( color.r, 0.f, 1.f );
        const float g = fClamp( color.g, 0.f, 1.f );
        const float b = fClamp( color.b, 0.f, 1.f );
        const float a = fClamp( color.a, 0.f, 1.f );

        pass.color.clear = { r, g, b, a };
        pass.clear_enabled = true_value;
        pass.context = context;

        if ( label )
        {
            strncpy( pass.label, label, Limits::LabelSize - 1 );
            pass.label[ Limits::LabelSize - 1] = '\0';
        }

        return pass;
    }

}
