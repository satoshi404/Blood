#pragma once

#include <pipeline.hpp>
#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// xcb/xproto.h

#if PLATFORM_LINUX

enum_type( XCB_PropModeType, uint_32 )
{
	XCB_PropModeType_Replace = 0,
	XCB_PropModeType_Prepend = 1,
	XCB_PropModeType_Append = 2,
};

#if VENDOR_XCB

	#define XCB_CW_BACK_PIXEL  ( 2 )
	#define XCB_CW_EVENT_MASK ( 2048 )

	#define XCB_EVENT_MASK_EXPOSURE ( 32768 )
	#define XCB_EVENT_MASK_KEY_PRESS ( 1 )

	#define XCB_ATOM_WM_NORMAL_HINTS ( 40 )
	#define XCB_ATOM_WM_SIZE_HINTS ( 41 )

	#define  XCB_WINDOW_CLASS_INPUT_OUTPUT ( 1 )

#else

	#include <xcb/xproto.h>

#endif

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////