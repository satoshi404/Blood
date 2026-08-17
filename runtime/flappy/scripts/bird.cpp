#include "bird.hpp"

#include <core/types.hpp>
#include <core/debug.hpp>

LOCAL const u32 size = 50;

void Bird::init()
{
	Debug::Println( PrintColor_Yellow, "Bird::init" );
}

void Bird::update()
{
	// ..
}

void Bird::bump()
{
	Debug::Println( PrintColor_Yellow, "Bird::Bump..." );
}

void Bird::free()
{
	// ..
}