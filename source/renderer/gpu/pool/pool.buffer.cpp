#include <renderer/gpu/pool/gpu.buffer.pool.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>

namespace
{
    struct Slot
    {
        Buffer value = {};
        u32 generation = 0;
        bool alive = false;
    };

    Slot g_slots[GpuLimits::MaxBuffers];
    bool g_initialized = false;
}

bool GpuBufferPool::init()
{
    if (g_initialized)
        return true_value;

    for (u32 i = 0; i < GpuLimits::MaxBuffers; ++i)
        g_slots[i] = {};

    g_initialized = true_value;
    return true_value;
}

void GpuBufferPool::shutdown()
{
    for (u32 i = 0; i < GpuLimits::MaxBuffers; ++i)
    {
        if (g_slots[i].alive)
            g_slots[i].value.destroy();
        g_slots[i] = {};
    }

    g_initialized = false_value;
}

GpuBufferHandle GpuBufferPool::create(const f32* data, u32 count)
{
    if (!g_initialized)
        init();

    for (u32 i = 0; i < GpuLimits::MaxBuffers; ++i)
    {
        Slot& s = g_slots[i];
        if (s.alive)
            continue;

        if (!s.value.create(data, count))
            return {};

        ++s.generation;
        if (s.generation == 0)
            s.generation = 1;

        s.alive = true;
        return { i, s.generation };
    }

    return {};
}

bool GpuBufferPool::destroy(GpuBufferHandle h)
{
    if (!h.is_valid() || h.index >= GpuLimits::MaxBuffers)
        return false;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return false;

    s.value.destroy();
    s.alive = false;
    return true;
}

GpuBuffer* GpuBufferPool::get(GpuBufferHandle h)
{
    if (!h.is_valid() || h.index >= GpuLimits::MaxBuffers)
        return nullptr;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return nullptr;

    return &s.value;
}