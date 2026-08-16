#include <core/debug.hpp>
#include <core/types.hpp>

#include <platform/keyboard.hpp>
#include <engine/engine.hpp>

void _start()
{
	Debug::Println( PrintColor_Cyan, "Start.." );
}

void _input()
{
	if ( Keyboard::check_pressed( VK_U ) )
	{
		Debug::Println( PrintColor_Cyan, "Pressed" );
	}
	else if ( Keyboard::check_released( VK_U ) )
	{
		Debug::Println( PrintColor_Cyan, "Released" );
	}
}

void _update( f64 delta )
{
	// ..
}

void _finish()
{
	Debug::Println( PrintColor_Cyan, "Finish.." );
}