#pragma once

#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/buffer.hpp>

namespace BufferPool
{
    bool init();
    void shutdown();

    BufferHandle create(const float_32* data, uint_32 float_count);
    bool destroy( BufferHandle handle );

    Buffer* get( BufferHandle handle );
}
