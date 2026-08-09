#pragma once

#include <renderer/gpu/core/gpu.handles.hpp>

struct GpuShader
{
    u32 native_id = 0;

    bool is_valid() const
    {
        return native_id != 0;
    }
};
