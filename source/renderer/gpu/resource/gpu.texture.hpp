#pragma once

#include <renderer/gpu/core/gpu.handles.hpp>

struct GpuTexture
{
    u32 native_id = 0;
    u32 width = 0;
    u32 height = 0;
    u32 channels = 0;

    bool is_valid() const
    {
        return native_id != 0;
    }
};
