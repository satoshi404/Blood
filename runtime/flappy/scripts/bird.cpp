#include <core/debug.hpp>
#include <core/types.hpp>

#include <platform/keyboard.hpp>

#include <engine/system/node.hpp>

GLOBAL void _start()
{
	Debug::Println( PrintColor_Cyan, "Start.." );
}

GLOBAL void _input()
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

GLOBAL void _draw( /*const Node self*/ )
{
	// self.pos( 0, 0 );
	// self.size( 50, 50 );
}

GLOBAL void _update( f64 delta )
{
	// ..
}

GLOBAL void _finish()
{
	Debug::Println( PrintColor_Cyan, "Finish.." );
}