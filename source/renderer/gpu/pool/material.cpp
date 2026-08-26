#include <renderer/gpu/pool.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <core/debug.hpp>

static Slot<Material> slots[ Limits::Max_Materials ] = {};


// TODO: Init
static bool initialized = true_value;

// Todo:
static bool valid_handle( MaterialHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < Limits::Max_Materials &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}

// TODO: init all
//bool Pool::init()
//{
//    if ( initialized ) return true_value;
//
//    for ( Slot<Material>& slot : slots ) slot = {};
//
//    initialized = true_value;
//
//    return true_value;
//}
//
//void Pool::free()
//{
//    if ( !initialized ) return;
//
//    for ( Slot<Material>& slot : slots ) slot = {};
//
//    initialized = false_value;
//}

MaterialHandle Pool::create_material( const Material& material )
{
    if ( !initialized )
    {
        Debug::Println( PrintColorType_Red, "[Pool::Material] create antes de init" );
        return {};
    }

    for ( Slot<Material>& slot : slots )
    {
        if ( slot.alive ) continue;

        slot.generation++;
        if ( slot.generation == 0 )
            slot.generation = 1;

        slot.value = material;
        slot.alive = true_value;

        const uint_32 index = &slot - slots;
        return { index, slot.generation };
    }

    Debug::Println(
        PrintColorType_Red,
        "[MaterialPool] pool cheio (max %u)",
        Limits::Max_Materials
    );
    return {};
}

bool Pool::update_material( MaterialHandle handle, const Material& material )
{
    if ( !valid_handle( handle ) ) return false_value;

    slots[ handle.index ].value = material;
    return true_value;
}

bool Pool::destroy_material( MaterialHandle handle )
{
    if ( !valid_handle( handle ) ) return false_value;

    slots[ handle.index ].value = {};
    slots[ handle.index ].alive = false_value;

    return true_value;
}

Material* Pool::get_material( MaterialHandle handle )
{
    return valid_handle( handle ) ? & slots[ handle.index ].value : nullptr;
}

bool Pool::exist_material( MaterialHandle handle )
{
    return valid_handle( handle );
}

//uint_32 MaterialPool::size()
//{
//    return g_count;
//}
//
//uint_32 MaterialPool::capacity()
//{
//    return Limits::Max_Materials;
//}