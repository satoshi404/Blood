#include <renderer/gpu/pool/gpu.buffer.pool.hpp>
#include <renderer/gpu/core/limits.hpp>

namespace
{
    struct Slot
    {
        Buffer value = {};
        uint_32 generation = 0;
        bool alive = false;
    };

    Slot g_slots[Limits::Max_Buffers];
    bool g_initialized = false_value;
}

bool BufferPool::init()
{
    if (g_initialized)
        return true_value;

    for (uint_32 i = 0; i < Limits::Max_Buffers; ++i)
        g_slots[i] = {};

    g_initialized = true_value;
    return true_value;
}

void BufferPool::shutdown()
{
    for (uint_32 i = 0; i < Limits::Max_Buffers; ++i)
    {
        if (g_slots[i].alive)
            g_slots[i].value.destroy();
        g_slots[i] = {};
    }

    g_initialized = false_value;
}

BufferHandle BufferPool::create(const float_32* data, uint_32 count)
{
    if (!g_initialized)
        init();

    for (uint_32 i = 0; i < Limits::Max_Buffers; ++i)
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

bool BufferPool::destroy( BufferHandle h )
{
    if (!h.is_valid() || h.index >= Limits::Max_Buffers)
        return false;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return false;

    s.value.destroy();
    s.alive = false;
    return true;
}

Buffer* BufferPool::get( BufferHandle h )
{
    if (!h.is_valid() || h.index >= Limits::Max_Buffers)
        return nullptr;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return nullptr;

    return &s.value;
}