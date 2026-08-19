#pragma once

#include <core/types.hpp>

#include <renderer/gpu/core/gpu.limits.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/resource/gpu.descriptor.hpp>

struct GpuRenderQueue
{
	char label[GpuLimits::LabelSize] = {};

	GpuDescriptorHandle items[ GpuLimits::MaxDescriptors ] = {};
	u32 count = 0;

	bool sort_by_material = true_value;
	bool sort_by_depth = true_value;
	bool sort_by_layer = true_value;

	void clear()
	{
		count = 0;
	}

	bool push( GpuDescriptorHandle handle )
	{
		if ( count >= GpuLimits::MaxDescriptors || !handle.is_valid() ) return false_value;

		items[count++] = handle;
		return true_value;
	}

	bool sort() const
	{
		return count == 0;
	}
};