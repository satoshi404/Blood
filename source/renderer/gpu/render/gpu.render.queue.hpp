#pragma once

#include <core/types.hpp>

#include <renderer/gpu/core/limits.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/gpu.descriptor.hpp>

struct RenderQueue
{
	char label[ Limits::Label_Size ] = {};

	DescriptorHandle items[ Limits::Max_Descriptors ] = {};
	uint_32 count = 0;

	bool sort_by_material = true_value;
	bool sort_by_depth = true_value;
	bool sort_by_layer = true_value;

	void clear()
	{
		count = 0;
	}

	bool push( DescriptorHandle handle )
	{
		if ( count >= Limits::Max_Descriptors || !handle.is_valid() ) return false_value;

		items[count++] = handle;
		return true_value;
	}

	bool sort() const
	{
		return count == 0;
	}
};