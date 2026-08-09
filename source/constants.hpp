#pragma once

#include <pipeline.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define API_NAKED  ( 0 )
#define API_OPENGL ( 1 )
#define API_VULKAN ( 0 )
#define API_D3D12  ( 0 )

#define RENDERER ( API_NAKED || API_D3D12 || API_OPENGL || API_VULKAN )

#if API_NAKED

	#if PLATFORM_WINDOWS
		#define WINDOWS_NAKED_API ( 1 )
		#define LINUX_NAKED_API   ( 0 )
	#else
		#define WINDOWS_NAKED_API ( 0 )
		#define LINUX_NAKED_API   ( 1 )
	#endif

#elif API_VULKAN

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
#define VENDOR_XCB_ALLSUBMODELS ( 0 )

#if VENDOR_XCB && VENDOR_XCB_ALLSUBMODELS
	#define VENDOR_XCB_KEYSYMS ( 0 )
	#define VENDOR_XCB_XPROTO  ( 0 )
	#define VENDOR_XCB_ALL     ( 0 )
#endif

// X11
#define VENDOR_X11 ( 0 )
#define VENDOR_X11_ALLSUBMODELS ( 0 )

// X11 submodules
#if VENDOR_X11 && VENDOR_X11_ALLSUBMODELS
	#define VENDOR_X11_XUTIL ( 0 )
	#define VENDOR_X11_ALL   ( 0 )
#endif

// WINDOWS API
#define VENDOR_WINDOWS 	( 0 )
#define VENDOR_WINDOWS_ALLSUBMODELS ( 0 )

#if VENDOR_WINDOWS && VENDOR_WINDOWS_ALLSUBMODELS
	// ..
#endif

// LIBC
#define VENDOR_STDIO 	( 0 )
#define VENDOR_STDLIB 	( 0 )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
