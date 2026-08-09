#pragma once

#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/resource/gpu.texture.hpp>

namespace GpuTexturePool
{
    bool init();
    void shutdown();

    GpuTextureHandle create(const GpuTexture& texture);
    bool destroy(GpuTextureHandle handle);

    GpuTexture* get(GpuTextureHandle handle);
   // const GpuTexture* get(GpuTextureHandle handle);
}
