#include <renderer/gpu/pool/shader.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <core/debug.hpp>

namespace
{
    struct Slot
    {
        Shader value      = {};
        uint_32  generation = 0;
        bool     alive      = false_value;
    };

    Slot    g_slots[ Limits::Max_Shaders ];
    uint_32 g_count       = 0;
    bool    g_initialized = false_value;

    bool valid_handle( ShaderHandle handle )
    {
        return handle.is_valid()
            && handle.index < Limits::Max_Shaders
            && g_slots[ handle.index ].alive
            && g_slots[ handle.index ].generation == handle.generation;
    }
}

bool ShaderPool::init()
{
    if ( g_initialized )
        return true_value;

    for ( uint_32 i = 0; i < Limits::Max_Shaders; ++i )
        g_slots[ i ] = {};

    g_count       = 0;
    g_initialized = true_value;
    return true_value;
}

void ShaderPool::shutdown()
{
    if ( !g_initialized )
        return;

    for ( uint_32 i = 0; i < Limits::Max_Shaders; ++i )
        g_slots[ i ] = {};

    g_count       = 0;
    g_initialized = false_value;
}

ShaderHandle ShaderPool::create( const Shader& shader )
{
    if ( !g_initialized )
    {
        Debug::Println( PrintColorType_Red, "[ShaderPool] create antes de init" );
        return {};
    }

    for ( uint_32 i = 0; i < Limits::Max_Shaders; ++i )
    {
        Slot& slot = g_slots[ i ];
        if ( slot.alive )
            continue;

        slot.generation++;
        if ( slot.generation == 0 )
            slot.generation = 1;

        slot.value = shader;
        slot.alive = true_value;
        ++g_count;

        return { i, slot.generation };
    }

    Debug::Println(
        PrintColorType_Red,
        "[ShaderPool] pool cheio (max %u)",
        Limits::Max_Shaders
    );
    return {};
}

// bool ShaderPool::update( ShaderHandle handle, const Shader& shader )
// {
//     if ( !valid_handle( handle ) )
//         return false_value;

//     g_slots[ handle.index ].value = shader;
//     return true_value;
// }

bool ShaderPool::destroy( ShaderHandle handle )
{
    if ( !valid_handle( handle ) )
        return false_value;

    g_slots[ handle.index ].value = {};
    g_slots[ handle.index ].alive = false_value;

    if ( g_count > 0 )
        --g_count;

    return true_value;
}

Shader* ShaderPool::get( ShaderHandle handle )
{
    return valid_handle( handle ) ? &g_slots[ handle.index ].value : nullptr;
}

const Shader* ShaderPool::get_const( ShaderHandle handle )
{
    return get( handle );
}

bool ShaderPool::exists( ShaderHandle handle )
{
    return valid_handle( handle );
}

uint_32 ShaderPool::size()
{
    return g_count;
}

uint_32 ShaderPool::capacity()
{
    return Limits::Max_Shaders;
}