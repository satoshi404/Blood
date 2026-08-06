#include <renderer/naked/naked.hpp>

#include <core/debug.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

#if PLATFORM_LINUX && API_NAKED

#include <vendor/xcb/xcb.hpp>

struct Handle
{
	void* connection;
	void* window;
	void* gc;
};

extern Handle handle;

void Naked::naked_draw_rectangle( Naked_Rect rect )
{
	const int16_t x = static_cast<int16_t>( rect.x );
	const int16_t y = static_cast<int16_t>( rect.y );

	const int16_t x1 = static_cast<int16_t>( rect.x + rect.w );
	const int16_t y1 = static_cast<int16_t>( rect.y );

	const int16_t x2 = static_cast<int16_t>( rect.x + rect.w );
	const int16_t y2 = static_cast<int16_t>( rect.y - rect.h);

	const int16_t x3 = static_cast<int16_t>( rect.x - rect.w );
	const int16_t y3 = static_cast<int16_t>( rect.y - rect.h );

	Debug::Println( PrintColor_Green, "Drawing..." );


	//xcb_point_t points[4] =
	//{
	//	{ x, y },
	//	{ x1, y1 },
	//	{ x2, y2 },
	//	{ x1, y2 },
	//};
//
	//xcb_fill_poly(
    //	(xcb_connection_t*)handle.connection,
    //	(u64)handle.window,
    //	(u64)handle.gc,
    //	XCB_POLY_SHAPE_CONVEX,           // Formato do polígono
    //	XCB_COORD_MODE_ORIGIN,           // Coordenadas relativas à origem
    //	4,                               // Número de pontos
	//	points
	//);

}

#endif