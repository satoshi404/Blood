#include <renderer/gpu/pool/descriptor.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <core/debug.hpp>

struct Slot
{
    Descriptor value = {};
    uint_32 generation = 0;
    bool alive = false_value;
};

static Slot slots[ Limits::Max_Descriptors ];
static uint_32  slot_size = 0;

static bool descriptor_initialised = false_value;

static bool valid_handler( DescriptorHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < Limits::Max_Descriptors &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}

bool DescriptorPool::init()
{
    for ( uint_32 i = 0; i < Limits::Max_Descriptors; ++i ) slots[ i ] = {};

    slot_size = 0;

    descriptor_initialised = true_value;

    return true_value;
}

void DescriptorPool::shutdown()
{
    if ( !descriptor_initialised )
    {
        Debug::Println( PrintColorType_Red, "Error: Descriptor not initialised" );
        return;
    }
    slot_size = 0;

    for (uint_32 i = 0; i < Limits::Max_Descriptors; ++i) slots[i] = {};
}

DescriptorHandle DescriptorPool::create(const Descriptor &descriptor)
{
    if ( !descriptor_initialised )
    {
        Debug::Println( PrintColorType_Red, "Error: Descriptor not initialised" );
        return {};
    }

    for ( uint_32 i = 0; i < Limits::Max_Descriptors; ++i )
    {
        Slot &slot = slots[i];

        if (slot.alive)
            continue;

        slot.generation++;
        if (slot.generation == 0)
            slot.generation = 1;

        slot.value = descriptor;
        slot.alive = true_value;
        ++slot_size;

        return {i, slot.generation};
    }

    Debug::Println(
        PrintColorType_Red,
        "[Gpu] Descriptor pool cheio (max %u)",
        Limits::Max_Descriptors);

    return {};
}

bool DescriptorPool::update(
    DescriptorHandle handle,
    const Descriptor &descriptor)
{
    if ( !descriptor_initialised )
    {
        Debug::Println( PrintColorType_Red, "Error: Descriptor not initialised" );
        return false_value;
    }

    if ( !valid_handler(handle) ) return false_value;

    slots[handle.index].value = descriptor;
    slots[handle.index].value.dirty = true;
    return true_value;
}

bool DescriptorPool::destroy( DescriptorHandle handle )
{
    if (!valid_handler(handle))
        return false_value;

    slots[handle.index].alive = false_value;
    slots[handle.index].value = {};

    if ( slot_size > 0 ) --slot_size;

    return true_value;
}

Descriptor *DescriptorPool::get( DescriptorHandle handle )
{
    return valid_handler(handle) ? &slots[ handle.index ].value : nullptr;
}

bool DescriptorPool::exists( DescriptorHandle handle )
{
    return valid_handler(handle);
}

uint_32 DescriptorPool::size()
{
    return slot_size;
}

uint_32 DescriptorPool::capacity()
{
    return Limits::Max_Descriptors;
}
