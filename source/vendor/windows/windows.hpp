#pragma once

#include <core/types.hpp>
#include <pipeline.hpp>
#include <constants.hpp>

// TODO:
#define T3( x ) L##x
#define TE( x ) T3( x )

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Windows.h

#if PLATFORM_WINDOWS

#if VENDOR_WINDOWS

#else
 	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <windowsx.h>
	#include <winuser.h>
	#include <winbase.h>
	#include <shlobj.h>

#endif

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////