#pragma once

#include <renderer/gpu/render/gpu.render.queue.hpp>
#include <renderer/gpu/core/limits.hpp>

// TODO:
#include <string.h>

namespace RenderQueuePool
{
	extern bool init();
	extern void shutdown();

	extern RenderQueueHandle create( const char* label = nullptr );
	extern bool destroy( RenderQueueHandle handle );

	extern RenderQueue* get( RenderQueueHandle handle );
	extern bool exists( RenderQueueHandle handle );

	extern uint_32 size();
	extern uint_32 capacity();
}