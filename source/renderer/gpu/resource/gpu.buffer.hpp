#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <core/types.hpp>

struct Buffer
{
    GpuBufferType type = GpuBufferType_Vertex;

    // Backend-owned native handle. Kept generic here.
    u32 native_id = 0;

    u32 float_count = 0;
    usize byte_size = 0;

    bool create(const f32* data, u32 float_count);
    void destroy();

    bool is_valid() const
    {
        return native_id != 0;
    }
};
