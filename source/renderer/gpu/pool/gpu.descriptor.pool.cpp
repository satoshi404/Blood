#include <renderer/gpu/pool/gpu.descriptor.pool.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>
#include <core/debug.hpp>

struct Slot
{
    GpuDescriptor value = {};
    u32 generation = 0;
    bool alive = false_value;
};

static Slot slots[ GpuLimits::MaxDescriptors ];
static u32  slot_size = 0;

static bool descriptor_initialised = false_value;

static bool valid_handler( GpuDescriptorHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < GpuLimits::MaxDescriptors &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}

bool GpuDescriptorPool::init()
{
    for ( u32 i = 0; i < GpuLimits::MaxDescriptors; ++i ) slots[ i ] = {};

    slot_size = 0;

    descriptor_initialised = true_value;

    return true_value;
}

void GpuDescriptorPool::shutdown()
{
    if ( !descriptor_initialised )
    {
        Debug::Println( PrintColor_Red, "Error: Descriptor not initialised" );
        return;
    }
    slot_size = 0;

    for (u32 i = 0; i < GpuLimits::MaxDescriptors; ++i) slots[i] = {};
}

GpuDescriptorHandle GpuDescriptorPool::create(const GpuDescriptor &descriptor)
{
    if ( !descriptor_initialised )
    {
        Debug::Println( PrintColor_Red, "Error: Descriptor not initialised" );
        return {};
    }

    for ( u32 i = 0; i < GpuLimits::MaxDescriptors; ++i )
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
        PrintColor_Red,
        "[Gpu] Descriptor pool cheio (max %u)",
        GpuLimits::MaxDescriptors);

    return {};
}

bool GpuDescriptorPool::update(
    GpuDescriptorHandle handle,
    const GpuDescriptor &descriptor)
{
    if ( !descriptor_initialised )
    {
        Debug::Println( PrintColor_Red, "Error: Descriptor not initialised" );
        return false_value;
    }

    if ( !valid_handler(handle) ) return false_value;

    slots[handle.index].value = descriptor;
    slots[handle.index].value.dirty = true;
    return true_value;
}

bool GpuDescriptorPool::destroy(GpuDescriptorHandle handle)
{
    if (!valid_handler(handle))
        return false_value;

    slots[handle.index].alive = false_value;
    slots[handle.index].value = {};

    if ( slot_size > 0 ) --slot_size;

    return true_value;
}

GpuDescriptor *GpuDescriptorPool::get(GpuDescriptorHandle handle)
{
    return valid_handler(handle) ? &slots[ handle.index ].value : nullptr;
}

bool GpuDescriptorPool::exists(GpuDescriptorHandle handle)
{
    return valid_handler(handle);
}

u32 GpuDescriptorPool::size()
{
    return slot_size;
}

u32 GpuDescriptorPool::capacity()
{
    return GpuLimits::MaxDescriptors;
}
