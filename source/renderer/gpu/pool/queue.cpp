#include <renderer/gpu/pool.hpp>

#include <core/debug.hpp>
#include <vendor/libc/string.hpp>

static Slot<RenderQueue> slots[ Limits::Max_Layers ];

// TODO: Init
static bool initialized = true_value;

// Todo:
bool valid_handle( RenderQueueHandle handle )
{
    return
    (
        handle.is_valid() &&
        handle.index < Limits::Max_Layers &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation )
    );
}

//bool Pool::init()
//{
//    for ( Slot<RenderQueue>& slot : slots )
//	{
//		slot = {};
//		slot.generation = 0;
//		slot.alive = false_value;
//	}
//
//    initialized = true_value;
//
//    return true_value;
//}
//
//void Pool::free()
//{
//     for ( Slot<RenderQueue>& slot : slots )
//	 {
//		slot = {};
//	 }
//
//	 initialized = false_value;
//}

RenderQueueHandle Pool::create_queue( const char* label )
{

    for ( Slot<RenderQueue>& slot : slots )
    {
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

		const uint_32 index = &slot - slots;
		return { index, slot.generation };

	}
    return {};
}

bool Pool::destroy_queue( RenderQueueHandle handle )
{
    if (!exist_queue( handle )) return false_value;

	Slot<RenderQueue>& slot = slots[ handle.index ];

    slot.alive = false_value;
	slot.generation++;
    slot.value.clear();

    return true_value;
}

RenderQueue* Pool::get_queue( RenderQueueHandle handle )
{
	return valid_handle( handle ) ? &slots[ handle.index ].value : nullptr;
}

bool Pool::exist_queue( RenderQueueHandle handle )
{
   return  valid_handle( handle );
}

//signed_size Pool::size()
//{
//    return ARRAY_SIZE( slots );
//}
//
//signed_size Pool::capacity()
//{
//    return Limits::Max_Layers;
//}
