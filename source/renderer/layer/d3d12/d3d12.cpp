#include <renderer/layer/backend.hpp>

#if API_D3D12

#include <backend/d3d12/d3d12.hpp>

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
