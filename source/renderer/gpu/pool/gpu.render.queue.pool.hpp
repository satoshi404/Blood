#pragma once

#include <renderer/gpu/render/gpu.render.queue.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>

// TODO:
#include <string.h>

namespace GpuRenderQueuePool
{
	bool init();
	void shutdown();

	GpuRenderQueueHandle create( const char* label = nullptr );
	bool destroy( GpuRenderQueueHandle handle );

	GpuRenderQueue* get( GpuRenderQueueHandle handle );
	bool exists( GpuRenderQueueHandle handle );

	u32 size();
	u32 capacity();
}