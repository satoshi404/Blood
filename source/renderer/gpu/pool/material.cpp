#include <renderer/gpu/pool/material.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <core/debug.hpp>

namespace
{
    struct Slot
    {
        Material value      = {};
        uint_32  generation = 0;
        bool     alive      = false_value;
    };

    Slot    g_slots[ Limits::Max_Materials ];
    uint_32 g_count       = 0;
    bool    g_initialized = false_value;

    bool valid_handle( MaterialHandle handle )
    {
        return handle.is_valid()
            && handle.index < Limits::Max_Materials
            && g_slots[ handle.index ].alive
            && g_slots[ handle.index ].generation == handle.generation;
    }
}

bool MaterialPool::init()
{
    if ( g_initialized )
        return true_value;

    for ( uint_32 i = 0; i < Limits::Max_Materials; ++i )
        g_slots[ i ] = {};

    g_count       = 0;
    g_initialized = true_value;
    return true_value;
}

void MaterialPool::shutdown()
{
    if ( !g_initialized )
        return;

    for ( uint_32 i = 0; i < Limits::Max_Materials; ++i )
        g_slots[ i ] = {};

    g_count       = 0;
    g_initialized = false_value;
}

MaterialHandle MaterialPool::create( const Material& material )
{
    if ( !g_initialized )
    {
        Debug::Println( PrintColorType_Red, "[MaterialPool] create antes de init" );
        return {};
    }

    for ( uint_32 i = 0; i < Limits::Max_Materials; ++i )
    {
        Slot& slot = g_slots[ i ];
        if ( slot.alive )
            continue;

        slot.generation++;
        if ( slot.generation == 0 )
            slot.generation = 1;

        slot.value = material;
        slot.alive = true_value;
        ++g_count;

        return { i, slot.generation };
    }

    Debug::Println(
        PrintColorType_Red,
        "[MaterialPool] pool cheio (max %u)",
        Limits::Max_Materials
    );
    return {};
}

bool MaterialPool::update( MaterialHandle handle, const Material& material )
{
    if ( !valid_handle( handle ) )
        return false_value;

    g_slots[ handle.index ].value = material;
    return true_value;
}

bool MaterialPool::destroy( MaterialHandle handle )
{
    if ( !valid_handle( handle ) )
        return false_value;

    g_slots[ handle.index ].value = {};
    g_slots[ handle.index ].alive = false_value;

    if ( g_count > 0 )
        --g_count;

    return true_value;
}

Material* MaterialPool::get( MaterialHandle handle )
{
    return valid_handle( handle ) ? &g_slots[ handle.index ].value : nullptr;
}

const Material* MaterialPool::get_const( MaterialHandle handle )
{
    return get( handle );
}

bool MaterialPool::exists( MaterialHandle handle )
{
    return valid_handle( handle );
}

uint_32 MaterialPool::size()
{
    return g_count;
}

uint_32 MaterialPool::capacity()
{
    return Limits::Max_Materials;
}