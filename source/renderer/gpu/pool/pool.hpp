#pragma once

#include <renderer/gpu/core/handles.hpp>

#include <renderer/gpu/resource/gpu.descriptor.hpp>

#include <core/types.hpp>

template<typename T>
struct Slot
{
	T value = {};
    u32 generation = 0;
    bool alive = false_value;
};

namespace Pool
{
	extern BufferHandle create_buffer( const f32* data, u32 float_count );
	extern GpuDescriptorHandle create_descriptor( const GpuDescriptor &descriptor );
	extern void free();
};
