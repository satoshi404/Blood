#pragma once

#include <core/types.hpp>
#include <pipeline.hpp>

#include <vendor/xcb/xcb.hpp>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// X11/Xutil.h

#if PLATFORM_LINUX

// Flags Hints
#define X11_USPosition	( 1L << 0 )
#define X11_USSize		( 1L << 1 )

#define X11_PPosition	( 1L << 2 )
#define X11_PSize		( 1L << 3 )
#define X11_PMinSize	( 1L << 4 )
#define X11_PMaxSize	( 1L << 5 )
#define X11_PResizeInc	( 1L << 6 )
#define X11_PAspect		( 1L << 7 )
#define X11_PBaseSize	( 1L << 8 )
#define X11_PWinGravity	( 1L << 9 )

#define X11_PAllHints ( X11_PPosition | X11_PSize | X11_PMinSize | X11_PMaxSize | X11_PResizeInc | X11_PAspect )


	struct X11_SizeHints
	{
	    uint32_t flags;
	    int32_t x, y;
	    int32_t width, height;
	    int32_t min_width, min_height;
	    int32_t max_width, max_height;
	    int32_t width_inc, height_inc;
		struct
		{
			int x;
			int y;
		} min_aspect, max_aspect;
	    int32_t base_width, base_height;
	    int32_t win_gravity;
	};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void X11_SetFixedSize_Hints( XCB_Connection* conn, XCB_Window window, const int width, const int height );

#endif
