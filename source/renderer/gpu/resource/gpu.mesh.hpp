#pragma once

#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/gpu/core/gpu.enums.hpp>

struct GpuMesh
{
    GpuBuffer vertex_buffer;
    GpuBuffer index_buffer;

    u32 vertex_count = 0;
    u32 index_count = 0;
    u32 components = 2;
    TopologiePrimitive primitive = TopologiePrimitive_Default;

    bool upload( const f32* positions, u32 vertex_count, u32 components = 2 );
    void draw( TopologiePrimitive override = TopologiePrimitive_Default ) const;
    void destroy();

    bool is_valid() const
    {
        return vertex_buffer.is_valid() && vertex_count != 0;
    }
};
