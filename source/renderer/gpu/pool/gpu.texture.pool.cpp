#include <renderer/gpu/pool/gpu.texture.pool.hpp>
#include <renderer/gpu/core/limits.hpp>

namespace
{
    struct Slot
    {
        Texture value = {};
        uint_32 generation = 0;
        bool alive = false;
    };

    Slot g_slots[ Limits::Max_Textures ];
    bool g_initialized = false;
}

bool TexturePool::init()
{
    if (g_initialized)
        return true_value;

    for (uint_32 i = 0; i < Limits::Max_Textures; ++i)
        g_slots[i] = {};

    g_initialized = true_value;
    return true_value;
}

void TexturePool::shutdown()
{
    for (uint_32 i = 0; i < Limits::Max_Textures; ++i)
        g_slots[i] = {};

    g_initialized = false_value;
}

TextureHandle TexturePool::create( const Texture& texture )
{
    if (!g_initialized)
        init();

    for (uint_32 i = 0; i < Limits::Max_Textures; ++i)
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

bool TexturePool::destroy( TextureHandle handle )
{
    if (!handle.is_valid() || handle.index >= Limits::Max_Textures)
        return false_value;

    Slot& slot = g_slots[handle.index];

    if (!slot.alive || slot.generation != handle.generation)
        return false_value;

    slot.value = {};
    slot.alive = false_value;
    return true;
}

Texture* TexturePool::get( TextureHandle h )
{
    if (!h.is_valid() || h.index >= Limits::Max_Textures)
        return nullptr;

    Slot& s = g_slots[h.index];

    if (!s.alive || s.generation != h.generation)
        return nullptr;

    return &s.value;
}
