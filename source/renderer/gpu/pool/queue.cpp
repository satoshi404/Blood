#include <renderer/gpu/pool/queue.hpp>

//#include <renderer/gpu/pool/descriptor.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <core/debug.hpp>

namespace
{
struct Slot
{
    RenderQueue value = {};
    uint_32 generation = 0;
    bool alive = false_value;
};

 Slot slots[ Limits::Max_Layers ];
 uint_32  slot_size = 0;

bool valid_handler( RenderQueueHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < Limits::Max_Layers &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}
}

bool RenderQueuePool::init()
{
    for ( uint_32 i = 0; i < Limits::Max_Layers; ++i )
	{
		slots[ i ] = {};
		slots[ i ].generation = 0;
		slots[i].alive = false_value;
	}

    slot_size = 0;

    return true_value;
}

void RenderQueuePool::shutdown()
{
	slot_size = 0;
    for (uint_32 i = 0; i < Limits::Max_Layers; ++i) slots[i] = {};
}

RenderQueueHandle RenderQueuePool::create( const char* label )
{

    for ( uint_32 i = 0; i < Limits::Max_Layers; ++i )
    {
        Slot &slot = slots[i];

		if ( slot.alive ) continue;

		slot.generation++;

		if ( slot.generation == 0 ) slot.generation = 1;

		slot.alive = true_value;
		slot.value.clear();

		if ( label )
		{
			strncpy( slot.value.label, label, Limits::Label_Size - 1 );
			slot.value.label[ Limits::Label_Size - 1] = '\0';
		}
		else {
			slot.value.label[0] = '\0';
		}

		++slot_size;

		RenderQueueHandle handle;

		handle.index = i;
		handle.generation = slot.generation;
		return handle;

	}
    return {};
}

bool RenderQueuePool::destroy( RenderQueueHandle handle )
{
    if (!exists( handle ))
        return false_value;

    slots[handle.index].alive = false_value;
	slots[handle.index].generation++;
    slots[handle.index].value.clear();

    if ( slot_size > 0 ) --slot_size;

    return true_value;
}

RenderQueue *RenderQueuePool::get( RenderQueueHandle handle )
{
	return valid_handler( handle ) ? &slots[ handle.index ].value : nullptr;
}

bool RenderQueuePool::exists( RenderQueueHandle handle )
{
   return  valid_handler( handle );
}

uint_32 RenderQueuePool::size()
{
    return slot_size;
}

uint_32 RenderQueuePool::capacity()
{
    return Limits::Max_Layers;
}
