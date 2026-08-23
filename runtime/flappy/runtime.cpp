#include <core/debug.hpp>
#include <core/string.hpp>
#include <core/types.hpp>

#include <platform/keyboard.hpp>
#include <platform/window.hpp>

#include <engine/engine.hpp>
#include <engine/system/node.hpp>

#include "scripts/bird.hpp"

GLOBAL void _start()
{
	Debug::Println( PrintColorType_Cyan, "Start.." );

	String title;
	title.init( WindowConfig::Get::title() );
	title.append( " -" );
	title.append( "FlappyBirds", true_value );

	WindowConfig::Set::title( title.data() );
	CoreWindow::update_config();

	Bird::init();
}

GLOBAL void _input()
{
	if ( Keyboard::check_pressed( VK_Space ) )
	{
		Bird::bump();
	}
}

GLOBAL void _draw( /*const Node self*/ )
{
	// self.pos( 0, 0 );
	// self.size( 50, 50 );
}

GLOBAL void _update( float_64 delta )
{
	// ..
}

GLOBAL void _finish()
{
	Debug::Println( PrintColorType_Cyan, "Finish.." );

	Bird::free();
}