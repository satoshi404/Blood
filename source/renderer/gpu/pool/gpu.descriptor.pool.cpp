#include <renderer/gpu/pool/gpu.descriptor.pool.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>
#include <core/debug.hpp>

namespace
{
    struct Slot
    {
        GpuDescriptor value = {};
        u32 generation = 0;
        bool alive = false_value;
    };

    Slot g_slots[GpuLimits::MaxDescriptors];
    u32 g_count = 0;
    bool g_initialized = false_value;

    bool valid(GpuDescriptorHandle h)
    {
        return g_initialized
            && h.is_valid()
            && h.index < GpuLimits::MaxDescriptors
            && g_slots[h.index].alive
            && g_slots[h.index].generation == h.generation;
    }
}

bool GpuDescriptorPool::init()
{
    if (g_initialized)
        return true_value;

    for (u32 i = 0; i < GpuLimits::MaxDescriptors; ++i)
        g_slots[i] = {};

    g_count = 0;
    g_initialized = true_value;
    return true_value;
}

void GpuDescriptorPool::shutdown()
{
    g_initialized = false_value;
    g_count = 0;

    for (u32 i = 0; i < GpuLimits::MaxDescriptors; ++i)
        g_slots[i] = {};
}

GpuDescriptorHandle GpuDescriptorPool::create(const GpuDescriptor& descriptor)
{
    if (!g_initialized)
        init();

    for (u32 i = 0; i < GpuLimits::MaxDescriptors; ++i)
    {
        Slot& slot = g_slots[i];

        if (slot.alive)
            continue;

        slot.generation++;
        if (slot.generation == 0)
            slot.generation = 1;

        slot.value = descriptor;
        slot.alive = true_value;
        ++g_count;

        return { i, slot.generation };
    }

    Debug::Println(
        PrintColor_Red,
        "[Gpu] Descriptor pool cheio (max %u)",
        GpuLimits::MaxDescriptors
    );

    return {};
}

bool GpuDescriptorPool::update(
    GpuDescriptorHandle handle,
    const GpuDescriptor& descriptor)
{
    if (!valid(handle))
        return false_value;

    g_slots[handle.index].value = descriptor;
    g_slots[handle.index].value.dirty = true;
    return true_value;
}

bool GpuDescriptorPool::destroy(GpuDescriptorHandle handle)
{
    if (!valid(handle))
        return false_value;

    g_slots[handle.index].alive = false_value;
    g_slots[handle.index].value = {};

    if (g_count)
        --g_count;

    return true_value;
}

GpuDescriptor* GpuDescriptorPool::get(GpuDescriptorHandle handle)
{
    return valid(handle) ? &g_slots[handle.index].value : nullptr;
}

//const GpuDescriptor* GpuDescriptorPool::get(GpuDescriptorHandle handle)
//{
//    return valid(handle) ? &g_slots[handle.index].value : nullptr;
//}

bool GpuDescriptorPool::exists(GpuDescriptorHandle handle)
{
    return valid(handle);
}

u32 GpuDescriptorPool::size()
{
    return g_count;
}

u32 GpuDescriptorPool::capacity()
{
    return GpuLimits::MaxDescriptors;
}
