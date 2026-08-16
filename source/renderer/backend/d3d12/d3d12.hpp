#pragma once

#include <pipeline.hpp>

#if PLATFORM_WINDOWS
	// TODO: Better implementation
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#include <wrl/client.h>
#endif