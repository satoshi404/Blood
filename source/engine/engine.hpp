#pragma once

#include <core/types.hpp>

namespace Engine
{
	extern bool init();
	extern void update();
	extern void free();
}

namespace EngineCall
{
	extern void stop();
};

extern void _start();
extern void _input( /*const long keycode*/ );
extern void _update( f64 delta );
extern void _finish( );