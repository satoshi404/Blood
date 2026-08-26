#include <renderer/gpu/pool.hpp>
#include <renderer/gpu/core/limits.hpp>

static Slot<Buffer> slots[ Limits::Max_Buffers ] = {};
static bool initialized = false_value;

//bool Pool::init()
//{
//	if ( initialized ) return false_value;
//
//	for ( Slot<Buffer>& slot : slots ) {
//		slot = {};
//	}
//
//	initialized = true_value;
//
//	return true_value;
//}
//
//void Pool::shutdown()
//{
//	if ( !initialized ) return;
//
//	for ( Slot<Buffer>& slot : slots )
//    {
//        if ( slot.alive )
//            slot.value.destroy();
//
//        slot = {};
//    }
//}

BufferHandle Pool::create_buffer( const float_32* data, const uint_32 count )
{
	if ( !initialized ) init();

    for ( Slot<Buffer>& slot : slots )
    {
        if ( slot.alive ) continue;

        if ( !slot.value.create(data, count ) ) return {};

        ++slot.generation;
        if ( slot.generation == 0 ) slot.generation = 1;

        slot.alive = true_value;

		const uint_32 index = &slot - slots;
        return { index, slot.generation };
    }

    return {};
}

bool Pool::update_buffer( BufferHandle handle, const Buffer& resource )
{
    // ..
	return true_value;
}

Buffer* Pool::get_buffer( BufferHandle handle )
{
	if ( !handle.is_valid() || handle.index >= Limits::Max_Buffers ) return nullptr;

    Slot<Buffer>& slot = slots[ handle.index ];

    if ( !slot.alive || slot.generation != handle.generation )
        return nullptr;

    return &slot.value;
}

bool Pool::exist_buffer( BufferHandle handle )
{
    return  handle.is_valid() &&
        handle.index < Limits::Max_Buffers &&
        slots[handle.index].alive &&
        ( slots[handle.index].generation == handle.generation );
}

//signed_size Pool::size()
//{
//	return ARRAY_SIZE( slots );
//}
//
//signed_size Pool::capacity()
//{
//	return Limits::Max_Buffers;
//}

bool Pool::destroy_buffer( BufferHandle handle )
{
	if ( !handle.is_valid() || handle.index >= Limits::Max_Buffers ) return false_value;

    Slot<Buffer>& slot = slots[ handle.index ];

    if ( !slot.alive || slot.generation != handle.generation)
        return false;

    slot.value.destroy();
    slot.alive = false_value;

    return true_value;
}
