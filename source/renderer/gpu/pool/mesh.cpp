#include <renderer/gpu/pool.hpp>
#include <renderer/gpu/core/limits.hpp>

static Slot<Mesh> slots[ Limits::Max_Meshes ] = {};
static bool initialized = false_value;

//bool Pool::init()
//{
//	if ( initialized ) return false_value;
//
//	for ( Slot<Mesh>& slot : slots ) {
//		slot = {};
//	}
//
//	initialized = true_value;
//
//	return true_value;
//}
//
//void Pool::free()
//{
//	if ( !initialized ) return;
//
//	for ( Slot<Mesh>& slot : slots )
//    {
//        if ( slot.alive )
//            slot.value.destroy();
//
//        slot = {};
//    }
//}

MeshHandle Pool::create_mesh(  const float_32* positions, const uint_32 vertex_count, const uint_32 components )
{
	if ( !initialized ) init();

    for ( Slot<Mesh>& slot : slots )
    {
        if ( slot.alive ) continue;

        if ( !slot.value.upload( positions, vertex_count, components ) ) return {};

        ++slot.generation;
        if ( slot.generation == 0 ) slot.generation = 1;

        slot.alive = true_value;

		const uint_32 index = &slot - slots;
        return { index, slot.generation };
    }

    return {};
}

bool Pool::update_mesh( MeshHandle handle, const Mesh& mesh )
{
    // ..
	return true_value;
}

Mesh* Pool::get_mesh( MeshHandle handle )
{
	if ( !handle.is_valid() || handle.index >= Limits::Max_Meshes ) return nullptr;

    Slot<Mesh>& slot = slots[ handle.index ];

    if ( !slot.alive || slot.generation != handle.generation )
        return nullptr;

    return &slot.value;
}

bool Pool::exist_mesh( MeshHandle handle )
{
    return  handle.is_valid() &&
        handle.index < Limits::Max_Meshes &&
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
//	return Limits::Max_Meshes;
//}

bool Pool::destroy_mesh( MeshHandle handle )
{
	if ( !handle.is_valid() || handle.index >= Limits::Max_Buffers ) return false_value;

    Slot<Mesh>& slot = slots[ handle.index ];

    if ( !slot.alive || slot.generation != handle.generation)
        return false;

    slot.value.destroy();
    slot.alive = false_value;

    return true_value;
}
