#include <renderer/gpu.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

#if API_NAKED
#include <renderer/naked/naked.hpp>
#elif API_OPENGL
#include <renderer/opengl/opengl.hpp>
#include <renderer/opengl/gl.hpp>
#elif API_VULKAN
#include <renderer/vulkan/vulkan.hpp>
#endif

//static CommandType current_command;

namespace Cube
{
static void draw_2D(  )
{
#if API_NAKED
	Naked_Rect rect;
	// Test
	rect.w = 50;
	rect.h = 50;
	rect.x = 0;
	rect.y = 0;
	Naked::naked_draw_rectangle( rect );
#elif API_OPENGL

	glewInit();

	glBegin( GL_TRIANGLE_STRIP );
	glVertex2d( -5, 5 );
	glVertex2d( 5, 5 );
	glVertex2d( -5, -5 );
	glVertex2d( 5, -5 );
	glEnd();

#elif API_VULKAN
    // ..
#endif
}

static void draw_3D()
{
#if API_OPENGL
	// ..
#elif API_VULKAN
	// ..

//#elif API_NAKED
#endif
}
}


void GpuLayer::bind_command( GpuLayer_Command command_type )
{
	switch ( command_type.type )
	{
		case CommandType_Draw:
			switch ( command_type.draw.context )
			{
				case DrawContext_2D:

					switch ( command_type.draw.type )
					{
						case DrawCommandType_Cube:
							Cube::draw_2D();
						break;
					}

				break;
			}

		break;
	}
}