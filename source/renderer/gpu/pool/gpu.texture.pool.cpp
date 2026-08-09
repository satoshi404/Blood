#include <renderer/gpu/pool/gpu.texture.pool.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>

namespace
{
    struct Slot
    {
        GpuTexture value = {};
        u32 generation = 0;
        bool alive = false;
    };

    Slot g_slots[GpuLimits::MaxTextures];
    bool g_initialized = false;
}

bool GpuTexturePool::init()
{
    if (g_initialized)
        return true_value;

    for (u32 i = 0; i < GpuLimits::MaxTextures; ++i)
        g_slots[i] = {};

    g_initialized = true_value;
    return true_value;
}

void GpuTexturePool::shutdown()
{
    for (u32 i = 0; i < GpuLimits::MaxTextures; ++i)
        g_slots[i] = {};

    g_initialized = false_value;
}

GpuTextureHandle GpuTexturePool::create(const GpuTexture& texture)
{
    if (!g_initialized)
        init();

    for (u32 i = 0; i < GpuLimits::MaxTextures; ++i)
    {
        Slot& s = g_slots[i];
        if (s.alive)
            continue;

        ++s.generation;
        if (s.generation == 0)
            s.generation = 1;

        s.value = texture;
        s.alive = true;
        return { i, s.generation };
    }

    return {};
}

bool GpuTexturePool::destroy(GpuTextureHandle h)
{
    if (!h.is_valid() || h.index >= GpuLimits::MaxTextures)
        return false;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return false;

    s.value = {};
    s.alive = false;
    return true;
}

GpuTexture* GpuTexturePool::get(GpuTextureHandle h)
{
    if (!h.is_valid() || h.index >= GpuLimits::MaxTextures)
        return nullptr;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return nullptr;

    return &s.value;
}

//const GpuTexture* GpuTexturePool::get(GpuTextureHandle h)
//{
//    if (!h.is_valid() || h.index >= GpuLimits::MaxTextures)
//        return nullptr;
//
//    const Slot& s = g_slots[h.index];
//
//    if (!s.alive || s.generation != h.generation)
//        return nullptr;
//
//    return &s.value;
//}
