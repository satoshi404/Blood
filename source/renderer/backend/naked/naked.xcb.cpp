#include <renderer/backend/naked/naked.hpp>
#include <core/debug.hpp>
#include <pipeline.hpp>
#include <constants.hpp>
#include <platform/window.hpp>

#if PLATFORM_LINUX && API_NAKED

#include <vendor/xcb/xcb.hpp>

struct WindowHandler
{
	xcb_connection_t* connection;
	xcb_window_t window;
	xcb_gcontext_t gc;
};

extern WindowHandler window_handle;

// Helper to convert RGB to 32-bit pixel value safely
static inline uint32_t make_xcb_pixel(const u8 r, const u8 g, const u8 b)
{
	return (static_cast<uint32_t>(r) << 16)
	     | (static_cast<uint32_t>(g) << 8)
	     |  static_cast<uint32_t>(b);
}

void Naked::info_handle()
{
	Debug::Println( PrintColor_Yellow, "RENDERER NAKED XCB:" );
	Debug::Println( PrintColor_Yellow, "XCB Connection %p", static_cast<void*>(window_handle.connection) );
	Debug::Println( PrintColor_Yellow, "XCB GC 0x%08X", window_handle.gc );
	Debug::Println( PrintColor_Yellow, "XCB WINDOW 0x%08X", window_handle.window );
}

void Naked::draw_rectangle( Naked_Rect rect )
{
	xcb_rectangle_t xcb_rect =
	{
		static_cast<int16_t>( rect.x ),
		static_cast<int16_t>( rect.y ),
		static_cast<uint16_t>( rect.w ),
		static_cast<uint16_t>( rect.h ),
	};

	xcb_poly_fill_rectangle(
		window_handle.connection,
		window_handle.window,
		window_handle.gc,
		1,
		&xcb_rect
	);
}

void Naked::clear_color( const u8 r, const u8 g, const u8 b )
{
	// Temporarily set clear color to avoid corrupting the user's draw color state
	uint32_t clear_pixel = make_xcb_pixel(r, g, b);
	uint32_t mask = XCB_GC_FOREGROUND;
	uint32_t values[1] = { clear_pixel };
	xcb_change_gc( window_handle.connection, window_handle.gc, mask, values );

	xcb_rectangle_t full_window =
	{
		0, 0,
		static_cast<uint16_t>( WindowConfig::Get::width() ),
		static_cast<uint16_t>( WindowConfig::Get::height() )
	};

	xcb_poly_fill_rectangle( window_handle.connection, window_handle.window, window_handle.gc, 1, &full_window );
}

void Naked::set_draw_color( const u8 r, const u8 g, const u8 b )
{
	uint32_t pixel = make_xcb_pixel(r, g, b);
	uint32_t mask = XCB_GC_FOREGROUND;
	uint32_t values[1] = { pixel };

	xcb_change_gc( window_handle.connection, window_handle.gc, mask, values );
}

void Naked::swap()
{
	// Consider moving xcb_flush to your core event loop instead of every frame swap
	xcb_flush( window_handle.connection );
}

#endif
