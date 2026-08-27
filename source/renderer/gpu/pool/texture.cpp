#include <renderer/gpu/pool.hpp>
#include <renderer/gpu/core/limits.hpp>


static Slot<Texture> slots[ Limits::Max_Textures ];

// TODO: Init
static bool initialized = true_value; // false_value - but for test chenge to true

// Todo:
bool valid_handle( TextureHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < Limits::Max_Textures &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}

// TODO:
// bool Pool::init()
// {
//     if (initialized) return true_value;
//
//     for ( Slot<Texture>& slot : slots )
//         slot = {};
//
//     initialized = true_value;
//
//     return true_value;
// }
//
// void Pool::free()
// {
//     for ( Slot<Texture>& slot : slots )
//         slot = {};
//
//     initialized = false_value;
// }

TextureHandle Pool::create_texture( const Texture& texture )
{
    if ( !initialized ) init();

    for ( Slot<Texture>& slot : slots )
    {
        if ( slot.alive ) continue;

        ++slot.generation;
        if (slot.generation == 0)
            slot.generation = 1;

        slot.value = texture;
        slot.alive = true_value;

        const uint_32 index = &slot - slots;
        return { index, slot.generation };
    }

    return {};
}

bool Pool::update_texture( TextureHandle handle, const Texture& texture )
{
    if ( !valid_handle( handle ) )
        return false_value;

    Slot<Texture>& slot = slots[ handle.index ];
    slot.value = texture;

    return true_value;
}

bool Pool::destroy_texture( TextureHandle handle )
{
    if ( !handle.is_valid() || handle.index >= Limits::Max_Textures )
        return false_value;

    Slot<Texture>& slot = slots[handle.index];

    if (!slot.alive || slot.generation != handle.generation)
        return false_value;

    slot.value = {};
    slot.alive = false_value;

    return true_value;
}

Texture* Pool::get_texture( TextureHandle handle )
{
    if ( !handle.is_valid() || handle.index >= Limits::Max_Textures )
        return nullptr;

    Slot<Texture>& slot = slots[ handle.index ];

    if ( !slot.alive || slot.generation != handle.generation )
        return nullptr;

    return &slot.value;
}


bool Pool::exist_texture( TextureHandle handle )
{
    return valid_handle( handle );
}

// signed_size Pool::size()
// {
//     return ARRAY_SIZE( slots );
// }
//
// signed_size Pool::capacity()
// {
//     return Limits::Max_Textures;
// }
