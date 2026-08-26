#include <renderer/gpu/pool.hpp>
#include <renderer/gpu/core/limits.hpp>

#include <core/debug.hpp>

static Slot<Descriptor> slots[ Limits::Max_Descriptors ] = {};
static bool initialized = false_value;

// Todo:
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

bool Pool::init()
{
	if ( initialized ) return false_value;

	for ( Slot<Descriptor>& slot : slots ) {
		slot = {};
	}

	initialized = true_value;

	return true_value;
}

void Pool::free()
{
	if ( !initialized ) return;

	for ( Slot<Descriptor>& slot : slots ) slot = {};
}

DescriptorHandle Pool::create_descriptor( const Descriptor& descriptor )
{
	if ( !initialized )
    {
        Debug::Println( PrintColorType_Red, "[Pool::Error] Descriptor not initialized" );
        return {};
    }

    for ( Slot<Descriptor>& slot : slots )
    {
        if ( slot.alive ) continue;

        slot.generation++;
        if (slot.generation == 0)
            slot.generation = 1;

        slot.value = descriptor;
        slot.alive = true_value;

		const uint_32 index = &slot - slots;
        return { index, slot.generation };
    }

    Debug::Println(
        PrintColorType_Red,
        "[Pool::Error] Descriptor pool cheio (max %u)",
        Limits::Max_Descriptors);

    return {};
}

bool Pool::update_descriptor( DescriptorHandle handle, const Descriptor& descriptor )
{
    if ( !initialized )
    {
        Debug::Println( PrintColorType_Red, "[Pool::Error] Descriptor not initialised" );
        return false_value;
    }

    if ( !valid_handler(handle) ) return false_value;

    slots[handle.index].value = descriptor;
    slots[handle.index].value.dirty = true_value;

    return true_value;
}

Descriptor* Pool::get_descriptor( DescriptorHandle handle )
{
	if ( !handle.is_valid() || handle.index >= Limits::Max_Descriptors ) return nullptr;

    Slot<Descriptor>& slot = slots[ handle.index ];

    if ( !slot.alive || slot.generation != handle.generation )
        return nullptr;

    return &slot.value;
}

bool Pool::exist_descriptor( DescriptorHandle handle )
{
    return valid_handler( handle );
}

signed_size Pool::size()
{
	return ARRAY_SIZE( slots );
}

signed_size Pool::capacity()
{
	return Limits::Max_Descriptors;
}

bool Pool::destroy_descriptor( DescriptorHandle handle )
{
	if (!valid_handler(handle)) return false_value;

    slots[handle.index].alive = false_value;
    slots[handle.index].value = {};

    return true_value;
}
