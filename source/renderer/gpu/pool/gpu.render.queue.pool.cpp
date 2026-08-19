#include <renderer/gpu/pool/gpu.render.queue.pool.hpp>

#include <renderer/gpu/pool/gpu.descriptor.pool.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>
#include <core/debug.hpp>

namespace
{
struct Slot
{
    GpuRenderQueue value = {};
    u32 generation = 0;
    bool alive = false_value;
};

 Slot slots[ GpuLimits::MaxDescriptors ];
 u32  slot_size = 0;

bool valid_handler( GpuRenderQueueHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < GpuLimits::MaxLayers &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}
}

bool GpuRenderQueuePool::init()
{
    for ( u32 i = 0; i < GpuLimits::MaxLayers; ++i )
	{
		slots[ i ] = {};
		slots[ i ].generation = 0;
		slots[i].alive = false_value;
	}

    slot_size = 0;

    return true_value;
}

void GpuRenderQueuePool::shutdown()
{
	slot_size = 0;
    for (u32 i = 0; i < GpuLimits::MaxLayers; ++i) slots[i] = {};
}

GpuRenderQueueHandle GpuRenderQueuePool::create( const char* label )
{

    for ( u32 i = 0; i < GpuLimits::MaxLayers; ++i )
    {
        Slot &slot = slots[i];

		if ( slot.alive ) continue;

		slot.generation++;

		if ( slot.generation == 0 ) slot.generation = 1;

		slot.alive = true_value;
		slot.value.clear();

		if ( label )
		{
			strncpy( slot.value.label, label, GpuLimits::LabelSize - 1 );
			slot.value.label[GpuLimits::LabelSize - 1] = '\0';
		}
		else {
			slot.value.label[0] = '\0';
		}

		++slot_size;

		GpuRenderQueueHandle handle;

		handle.index = i;
		handle.generation = slot.generation;
		return handle;

	}
    return {};
}

bool GpuRenderQueuePool::destroy( GpuRenderQueueHandle handle )
{
    if (exists( handle ))
        return false_value;

    slots[handle.index].alive = false_value;
	slots[handle.index].generation++;
    slots[handle.index].value.clear();

    if ( slot_size > 0 ) --slot_size;

    return true_value;
}

GpuRenderQueue *GpuRenderQueuePool::get(GpuRenderQueueHandle handle)
{
	return valid_handler( handle ) ? &slots[ handle.index ].value : nullptr;
}

bool GpuRenderQueuePool::exists(GpuRenderQueueHandle handle)
{
   return  valid_handler( handle );
}

u32 GpuRenderQueuePool::size()
{
    return slot_size;
}

u32 GpuRenderQueuePool::capacity()
{
    return GpuLimits::MaxLayers;
}
