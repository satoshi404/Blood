// xcb.hpp
#pragma once

#include <pipeline.hpp>
#include <vendor/libc/stdint.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// xcb/xcb.h

#if PLATFORM_LINUX

#define XCB_PropModeReplace   0
#define XCB_AtomWmNormalHints 40
#define XCB_AtomWmSizeHints   41

#if VENDOR_XCB || VENDOR_ALL_XCB

    #define XCB_COPY_FROM_PARENT ( 0L )

    struct XCB_Connection
	{
		/* .. */
	};

    struct XCB_Window
	{
		 u32 handle;
	};

    struct XCB_Atom
	{
		u32 handle;
	};

    struct XCB_VoidCookie
	{
		/* .. */
	};

    struct XCB_GenericEvent
    {
        /* .. */
    };

    struct XCB_Setup
    {
        /* .. */
    };

    struct XCB_Screnn
    {

    };

    struct XCB_VisualID
    {

    };

    struct XCB_ScreenIterator
    {
        XCB_Screen *data;
        int rem;
        int index;
    };

    extern "C"
    {
        XCB_VoidCookie xcb_change_property(
            XCB_Connection *conn,
            uint8_t mode,
            XCB_Window window,
            XCB_Atom property,
            XCB_Atom type,
            uint8_t format,
            uint32_t data_len,
            const void *data
        );

        XCB_Setup* xcb_get_setup(
            XCB_Connetion* conn
        );

        XCB_ScreenInteractor xcb_setup_roots_iterator(
            XCB_Setup* setup
        );

        XCB_VoidCookie xcb_create_window(
            XCB_Connection *  conn,
            uint8_t           depth,
            XCB_Window        wid,
            XCB_Window        parent,
            int16_t           x,
            int16_t           y,
            uint16_t          width,
            uint16_t          height,
            uint16_t          border_width,
            uint16_t          _class,
            XCB_VisualID      visual,
            uint32_t          value_mask,
            const uint32_t   *value_list
      );

      uint32_t xcb_generate_id(
        XCB_Connection *conn
      );

    }

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#else

    #include <xcb/xcb.h>

    // Commom types xcb
    using XCB_Connection = xcb_connection_t;
    using XCB_Window = xcb_window_t;
    using XCB_Atom = xcb_atom_t;
    using XCB_VoidCookie = xcb_void_cookie_t;
    using XCB_GenericEvent = xcb_generic_event_t;
    using XCB_Setup = xcb_setup_t;
    using XCB_Screen = xcb_screen_t;
    using XCB_ScreenIterator = xcb_screen_iterator_t;
    using XCB_VisualID = xcb_visualid_t;

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // PLATFORM_LINUX