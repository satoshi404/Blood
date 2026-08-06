#include <vendor/x11/xutil.hpp>
#include <vendor/libc/string.hpp>
#include <vendor/xcb/xproto.hpp>

#include <core/debug.hpp>

#if PLATFORM_LINUX

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void X11_SetFixedSize_Hints( XCB_Connection *conn, XCB_Window window, const int width, const int height )
{

#if VENDOR_X11_XUTIL
    Debug::println( PrintColor_Green, "[ Vendor::X11 ] Util" );
#endif

    X11_SizeHints hints;
    memset( &hints, 0, sizeof( hints ) );

    hints.flags = X11_PMinSize | X11_PMaxSize;
    hints.min_width  = width;
    hints.min_height = height;
    hints.max_width  = width;
    hints.max_height = height;

    constexpr i32 hint_len = sizeof( hints ) / 4;

    xcb_change_property(
        conn,
        XCB_PropModeType_Replace,
        window,
        XCB_Atom{ XCB_ATOM_WM_NORMAL_HINTS },
        XCB_Atom{ XCB_ATOM_WM_SIZE_HINTS },
        32,
        hint_len,
        &hints
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
