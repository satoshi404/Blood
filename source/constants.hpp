#pragma once

#include <pipeline.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define API_OPENGL ( 1 )
#define API_VULKAN ( 0 )
#define API_D3D12  ( 0 )

#define RENDERER ( API_D3D12 || API_OPENGL || API_VULKAN )

#if !RENDERER
	#error "Select a api"
#endif

#if API_VULKAN

	#if PLATFORM_WINDOWS
		#define WINDOWS_VULKAN_API ( 1 )
		#define LINUX_VULKAN_API   ( 0 )
	#else
		#define WINDOWS_VULKAN_API ( 0 )
		#define LINUX_VULKAN_API   ( 1 )
	#endif

#elif API_OPENGL

	#if PLATFORM_WINDOWS
		#define WINDOWS_VULKAN_API ( 1 )
		#define LINUX_VULKAN_API   ( 0 )
	#else
		#define WINDOWS_VULKAN_API ( 0 )
		#define LINUX_VULKAN_API   ( 1 )
	#endif

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WINDOW_DEFAULT_HEIGHT ( 600 )
#define WINDOW_DEFAULT_WIDTH  ( 800 )
#define WINDOW_DEFAULT_TITLE  ( "AxelEngine" )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// XCB
#define VENDOR_XCB 		        ( 0 )

// X11
#define VENDOR_X11 ( 0 )

// WINDOWS API
#define VENDOR_WINDOWS 	( 0 )

// LIBC
#define VENDOR_STDIO 	( 0 )
#define VENDOR_STDLIB 	( 0 )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
