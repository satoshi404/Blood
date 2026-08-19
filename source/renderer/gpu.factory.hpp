#pragma once

#include <renderer/gpu/gpu.hpp>
#include <renderer/layer/gpu.backend.hpp>

namespace GpuFactory
{
    inline bool init() { return Gpu::init(); }
    inline void shutdown() { Gpu::shutdown(); }

    inline void new_frame() { Gpu::new_frame(); }
    inline const GpuStatistics& statistics() { return Gpu::statistics(); }

    inline GpuDescriptorHandle create_descriptor(const GpuDescriptor& d)
    {
        return Gpu::create_descriptor(d);
    }

    inline bool update_descriptor(GpuDescriptorHandle h, const GpuDescriptor& d)
    {
        return Gpu::update_descriptor(h, d);
    }

    inline bool destroy_descriptor(GpuDescriptorHandle h)
    {
        return Gpu::destroy_descriptor(h);
    }

    inline const GpuDescriptor* get_descriptor(GpuDescriptorHandle h)
    {
        return Gpu::get_descriptor(h);
    }

    inline GpuDescriptor* get_descriptor_mutable(GpuDescriptorHandle h)
    {
        return Gpu::get_descriptor_mutable( h );
    }

    inline GpuRenderQueueHandle create_render_queue( const char* label = nullptr )
    {
        return GpuRenderQueuePool::create( label );
    }

    inline bool destroy_render_queue( GpuRenderQueueHandle h )
    {
        return GpuRenderQueuePool::destroy( h );
    }

    inline GpuRenderQueue* get_render_queue(GpuRenderQueueHandle h )
    {
        return GpuRenderQueuePool::get( h );
    }

    inline GpuCommand make_draw_command(
        GpuDescriptorHandle h,
        const char* label = nullptr)
    {
        return GpuCommand::draw(h, label);
    }

    inline GpuCommand make_clear_command(
        f32 r, f32 g, f32 b, f32 a = 1.0f,
        const char* label = nullptr)
    {
        return GpuCommand::clear(r, g, b, a, label);
    }

    inline GpuCommand make_viewport_command(
        i32 x, i32 y, i32 width, i32 height,
        const char* label = nullptr)
    {
        return GpuCommand::viewport(x, y, width, height, label);
    }

    inline GpuCommand make_swap_command(const char* label = nullptr)
    {
        return GpuCommand::swap(label);
    }

    inline GpuCommand make_transform_command(
        f32 x, f32 y, f32 z,
        const char* label = nullptr)
    {
        GpuTransform t = {};
        t.position[0] = x;
        t.position[1] = y;
        t.position[2] = z;
        return GpuCommand::transform(t, label);
    }

    // TODO: temporary ( update material )
    inline GpuCommand make_material_command(
        const GpuMaterialHandle material,
        const char* label = nullptr)
    {
        return GpuCommand::material( nullptr, label);
    }

    inline GpuCommand make_render_state_command(
        const GpuRenderState& state,
        const char* label = nullptr)
    {
        return GpuCommand::render_state(state, label);
    }

    inline GpuCommand make_texture_command(
        GpuTextureHandle texture,
        u32 slot,
        const char* label = nullptr)
    {
        return GpuCommand::texture(texture, slot, label);
    }

    inline void bind_command(GpuCommand command)
    {
        Gpu::execute(command);
    }

    inline void submit(GpuCommandList& list)
    {
        Gpu::submit(list);
    }

    inline GpuCommand make_begin_render_pass( const GpuRenderPass& pass, const char* label = nullptr )
    {
        return GpuCommand::begin_render_pass( pass, label );
    }

    inline GpuCommand make_end_render_pass( const char* label = nullptr )
    {
        return GpuCommand::end_render_pass( label );
    }

    inline GpuCommand execute_queue( const GpuRenderQueueHandle& queue, const char* label = nullptr )
    {
        return GpuCommand::execute_queue( queue, label );
    }

    inline GpuRenderPass make_simple_pass(
        i32 x, i32 y, i32 width, i32 height,
        GpuRenderQueueHandle queue,
        GpuContext context,
        f32 r, f32 g, f32 b, f32 a = 1.0,
        const char* label = nullptr
    )
    {
        GpuRenderPass pass = {};
        pass.viewport = { x, y, width, height };
        pass.queue = queue;
        pass.color.load_op = GpuLoadOp_Clear;
        pass.color.clear = { r, g, b, a };
        pass.clear_enabled = true_value;
        pass.context = context;

        if ( label )
        {
            strncpy( pass.label, label, GpuLimits::LabelSize - 1 );
            pass.label[GpuLimits::LabelSize - 1] = '\0';
        }

        return pass;
    }

}
