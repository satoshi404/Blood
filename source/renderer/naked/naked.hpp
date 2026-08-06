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
	void naked_draw_rectangle( Naked_Rect rect );
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