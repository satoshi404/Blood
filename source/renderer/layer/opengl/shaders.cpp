#include <renderer/layer/backend.hpp>

#include <core/debug.hpp>

bool Backend::shader_create( Shader& shader )
{
	return true_value;
}

void Backend::shader_destroy( Shader& shader )
{
	// TODO:
	Debug::Println( PrintColorType_Yellow, "TODO:" );
}