#pragma once

#include <core/types.hpp>

#include <core/math.hpp>

struct Color
{
    f32 r = .0f;
    f32 g = .0f;
    f32 b = .0f;
    f32 a = .0f;

    Color normalized()
    {
        return
        {
            fClamp( r, 0.f, 1.f ),
            fClamp( g, 0.f, 1.f ),
            fClamp( b, 0.f, 1.f ),
            fClamp( a, 0.f, 1.f ),
        }
    }

    Color& mix( Color other )
    {
        const float r_norm = fClamp( other.r, 0.f, 1.f );
        const float g_norm = fClamp( other.g, 0.f, 1.f );
        const float b_norm = fClamp( other.b, 0.f, 1.f );
        const float a_norm = fClamp( other.a, 0.f, 1.f );

        r = fClamp( r_norm + r, 0.f, 1.f );
        g = fClamp( g_norm + g, 0.f, 1.f );
        b = fClamp( b_norm + b, 0.f, 1.f );
        a = fClamp( a_norm + a, 0.f, 1.f );

        return *this;
    }

};

#define BlackColor ( Color ){ 0.f, 0.f, 0.f, 1.f }
#define RedColor ( Color ){ 1.f, 0.f, 0.f, 1.f }
#define GreenColor ( Color ){ 0.f, 1.f, 0.f, 1.f }
#define BlueColor ( Color ){ 0.f, 0.f, 1.f, 1.f }

#define DefaultClearColor BlackColor

struct GpuSize
{
    f32 width = 1.0f;
    f32 height = 1.0f;
    f32 depth = 1.0f;
};

struct Viewport
{
    i32 x = 0;
    i32 y = 0;
    i32 width = 0;
    i32 height = 0;
};

struct StatisticsFrame
{
    u32 draw_calls = 0;
    u32 vertex_count = 0;
    u32 buffer_maps = 0;
    u32 texture_binds = 0;
    u32 shader_binds = 0;

    void reset()
    {
        *this = {};
    }
};

struct Statistics
{
    StatisticsFrame frame = {};

    usize memory_swapchain = 0;
    usize memory_shaders = 0;
    usize memory_vertex_buffers = 0;
    usize memory_instance_buffers = 0;
    usize memory_index_buffers = 0;
    usize memory_uniform_buffers = 0;
    usize memory_constant_buffers = 0;
    usize memory_mutable_buffers = 0;
    usize memory_textures = 0;
    usize memory_render_targets = 0;

    usize total_memory() const
    {
        return memory_swapchain
             + memory_shaders
             + memory_vertex_buffers
             + memory_instance_buffers
             + memory_index_buffers
             + memory_uniform_buffers
             + memory_constant_buffers
             + memory_mutable_buffers
             + memory_textures
             + memory_render_targets;
    }

    void reset_frame()
    {
        frame.reset();
    }
};
