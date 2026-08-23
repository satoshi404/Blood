#pragma once

#include <renderer/gpu/resource/buffer.hpp>
#include <renderer/gpu/core/enums.hpp>

struct Mesh
{
    Buffer vertex_buffer;
    Buffer index_buffer;

    uint_32 vertex_count = 0;
    uint_32 index_count = 0;
    uint_32 components = 2;
    TopologiePrimitiveType primitive = TopologiePrimitiveType_Default;

    bool upload( const float_32* positions, uint_32 vertex_count, uint_32 components = 2 );
    void draw( TopologiePrimitiveType override = TopologiePrimitiveType_Default ) const;
    void destroy();

    bool is_valid() const
    {
        return vertex_buffer.is_valid() && vertex_count != 0;
    }
};
