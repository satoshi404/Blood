#pragma once

#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/resource/gpu.buffer.hpp>

namespace GpuBufferPool
{
    bool init();
    void shutdown();

    GpuBufferHandle create(const f32* data, u32 float_count);
    bool destroy(GpuBufferHandle handle);

    GpuBuffer* get(GpuBufferHandle handle);
}
