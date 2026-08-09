#pragma once

#include <core/types.hpp>

struct GpuColor
{
    f32 r = 1.0f;
    f32 g = 1.0f;
    f32 b = 1.0f;
    f32 a = 1.0f;
};

struct GpuSize
{
    f32 width = 1.0f;
    f32 height = 1.0f;
    f32 depth = 1.0f;
};

struct GpuViewport
{
    i32 x = 0;
    i32 y = 0;
    i32 width = 0;
    i32 height = 0;
};

struct GpuClear
{
    f32 r = 0.0f;
    f32 g = 0.0f;
    f32 b = 0.0f;
    f32 a = 1.0f;
};

struct GpuStatisticsFrame
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

struct GpuStatistics
{
    GpuStatisticsFrame frame = {};

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
