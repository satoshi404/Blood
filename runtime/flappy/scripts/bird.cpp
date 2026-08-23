#include "bird.hpp"

#include <core/types.hpp>
#include <core/debug.hpp>

LOCAL const uint_32 size = 50;

void Bird::init()
{
	UNUSED_VAR( size );
	Debug::Println( PrintColorType_Yellow, "Bird::init" );
}

void Bird::update()
{
	// ..
}

void Bird::bump()
{
	Debug::Println( PrintColorType_Yellow, "Bird::Bump..." );
}

void Bird::free()
{
	// ..
}