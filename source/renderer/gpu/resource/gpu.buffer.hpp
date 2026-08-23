#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <core/types.hpp>

struct Buffer
{
    BufferType type = BufferType_Vertex;

    // Backend-owned native handle. Kept generic here.
    uint_32 native_id = 0;

    uint_32 float_count = 0;
    unsigned_size byte_size = 0;

    bool create(const float_32* data, uint_32 float_count);
    void destroy();

    bool is_valid() const
    {
        return native_id != 0;
    }
};
