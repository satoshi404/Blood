#include <renderer/gpu/pool/gpu.render.queue.pool.hpp>

#include <renderer/gpu/pool/gpu.descriptor.pool.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>
#include <core/debug.hpp>

struct Slot
{
    GpuRenderQueue value = {};
    u32 generation = 0;
    bool alive = false_value;
};

static Slot slots[ GpuLimits::MaxDescriptors ];
static u32  slot_size = 0;


bool GpuRenderQueuePool::init()
{
    for ( u32 i = 0; i < GpuLimits::MaxLayers; ++i )
	{
		slots[ i ] = {};
		slots[ i ].generation = 1;
	}

    slot_size = 0;

    return true_value;
}

void GpuRenderQueuePool::shutdown()
{
	slot_size = 0;
    for (u32 i = 0; i < GpuLimits::MaxDescriptors; ++i) slots[i] = {};
}

GpuRenderQueueHandle GpuRenderQueuePool::create( const char* label )
{

    for ( u32 i = 0; i < GpuLimits::MaxLayers; ++i )
    {
        Slot &slot = slots[i];

		if ( !slot.alive )
		{
			slot.alive = true_value;
			slot.value.clear();

			if ( label )
			{
				stpncpy( slot.value.label, label, GpuLimits::LabelSize - 1 );
				slot.value.label[ GpuLimits::LabelSize ] = '\0';
			}

			GpuRenderQueueHandle handle;
			handle.index = i;
			handle.generation = slots->generation;
			++slot_size;
			return handle;
		}
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
	if (!exists(handle))
		return nullptr;

	return &slots[ handle.index ].value;
}

bool GpuRenderQueuePool::exists(GpuRenderQueueHandle handle)
{
    if ( !handle.is_valid() ||  handle.index >= GpuLimits::MaxLayers )
		return false_value;

	Slot &slot = slots[ handle.index ];
	return slot.alive && slot.generation == handle.generation;
}

u32 GpuRenderQueuePool::size()
{
    return slot_size;
}

u32 GpuRenderQueuePool::capacity()
{
    return GpuLimits::MaxLayers;
}
