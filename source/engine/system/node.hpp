#pragma once

#include <core/types.hpp>

IMPORT void _start();
IMPORT void _input( /*const long keycode*/ );
IMPORT void _update( float_64 delta );
IMPORT void _finish( );
IMPORT void _draw();

struct Node
{
	// ..
};