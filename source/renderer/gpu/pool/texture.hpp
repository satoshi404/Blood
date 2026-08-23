#pragma once

#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/texture.hpp>

namespace TexturePool
{
    bool init();
    void shutdown();

    TextureHandle create(const Texture& texture);
    bool destroy(TextureHandle handle);

    Texture* get(TextureHandle handle);

}
