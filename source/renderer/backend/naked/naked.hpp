#pragma once

#include <core/types.hpp>
#include <constants.hpp>

// TODO: vector2 / venctor3

#if API_NAKED

struct Naked_Rect
{
	float x;
	float y;
	u32 w;
	u32 h;
};

namespace Naked
{
	void draw_rectangle( Naked_Rect rect );
	void clear_color( const u8 r, const u8 g, const u8 b );
	extern void set_draw_color( const u8 r, const u8 g, const u8 b );
	void swap();
	void info_handle();
}

//struct Buffer
//{
//	u64 id;
//
//	void naked_api_bind( Buffer&buffer )
//	{
//		// ..
//	}
//};

#endif