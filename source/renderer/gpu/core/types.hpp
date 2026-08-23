#pragma once

#include <core/types.hpp>

#include <core/math.hpp>

struct Color
{
    float_32 r = .0f;
    float_32 g = .0f;
    float_32 b = .0f;
    float_32 a = .0f;

    Color& normalized()
    {
        r = float_clamp( r, 0.f, 1.f );
        g = float_clamp( g, 0.f, 1.f );
        b = float_clamp( b, 0.f, 1.f );
        a = float_clamp( a, 0.f, 1.f );
        return *this;
    }

    Color& plus( const Color other )
    {
        return *this;
    }

    Color& mix( const Color other )
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        this->normalized();
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
    float_32 width = 1.0f;
    float_32 height = 1.0f;
    float_32 depth = 1.0f;
};

struct Viewport
{
    int_32 x = 0;
    int_32 y = 0;
    int_32 width = 0;
    int_32 height = 0;
};

struct StatisticsFrame
{
    uint_32 draw_calls = 0;
    uint_32 vertex_count = 0;
    uint_32 buffer_maps = 0;
    uint_32 texture_binds = 0;
    uint_32 shader_binds = 0;

    void reset()
    {
        *this = {};
    }
};

struct Statistics
{
    StatisticsFrame frame = {};

    unsigned_size memory_swapchain = 0;
    unsigned_size memory_shaders = 0;
    unsigned_size memory_vertex_buffers = 0;
    unsigned_size memory_instance_buffers = 0;
    unsigned_size memory_index_buffers = 0;
    unsigned_size memory_uniform_buffers = 0;
    unsigned_size memory_constant_buffers = 0;
    unsigned_size memory_mutable_buffers = 0;
    unsigned_size memory_textures = 0;
    unsigned_size memory_render_targets = 0;

    unsigned_size total_memory() const
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
