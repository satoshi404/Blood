#include <renderer/layer/gpu.backend.hpp>

#if API_VULKAN

#include <backend/vulkan/vulkan.hpp>

bool GpuBackend::init()
{
	// ..
}


void GpuBackend::shutdown()
{
	// ..
}

void GpuBackend::clear( const GpuClear &c )
{
	// ..
}

void GpuBackend::viewport( const GpuViewport &vp )
{
	// ..
}

void GpuBackend::swap()
{
    CoreWindow::swap_buffers();
}

#endif
