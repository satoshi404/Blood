#include <renderer/gpu/pool.hpp>
#include <renderer/gpu/core/limits.hpp>

#include <core/debug.hpp>


static Slot<Shader> slots[ Limits::Max_Shaders ];

// TODO: Init
static bool initialized = true_value; // false_value - but for test chenge to true

// Todo:
bool valid_handle( ShaderHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < Limits::Max_Shaders &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}

// TODO:
// bool Pool::init()
// {
//     if ( initialized ) return true_value;
//
//     for ( Slot<Shader>& slot : slots  )
//         slot = {};
//
//     initialized = true_value;
//
//     return true_value;
// }
//
// void Pool::free()
// {
//     if ( !initialized ) return;
//
//     for ( Slot<Shader>& slot : slots  )
//         slot = {};
//
//     initialized = false_value;
// }

ShaderHandle Pool::create_shader( const Shader& shader )
{
    if ( !initialized )
    {
        Debug::Println( PrintColorType_Red, "[ShaderPool] create antes de init" );
        return {};
    }

    for ( Slot<Shader>& slot : slots  )
    {
        if ( slot.alive ) continue;

        slot.generation++;
        if ( slot.generation == 0 )
            slot.generation = 1;

        slot.value = shader;
        slot.alive = true_value;

        const uint_32 index = &slot - slots;
        return { index, slot.generation };
    }

    Debug::Println(
        PrintColorType_Red,
        "[ShaderPool] pool cheio (max %u)",
        Limits::Max_Shaders
    );
    return {};
}

bool Pool::update_shader( ShaderHandle handle, const Shader& shader )
{
    if ( !valid_handle( handle ) )
        return false_value;

    Slot<Shader>& slot = slots[ handle.index ];
    slot.value = shader;

    return true_value;
}

bool Pool::destroy_shader( ShaderHandle handle )
{
    if ( !valid_handle( handle ) )
        return false_value;

    Slot<Shader>& slot = slots[ handle.index ];
    slot.value = {};
    slot.alive = false_value;

    return true_value;
}

Shader* Pool::get_shader( ShaderHandle handle )
{
    return valid_handle( handle ) ? &slots[ handle.index ].value : nullptr;
}

bool Pool::exist_shader( ShaderHandle handle )
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
//     return Limits::Max_Shaders;
// }