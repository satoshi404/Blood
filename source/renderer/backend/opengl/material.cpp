#include <renderer/backend/backend.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

void Backend::bind_material( Material material )
{

    if (material.native_shader_id == 0)
    {
        const GLfloat color[4] =
        {
            material.color.r,
            material.color.g,
            material.color.b,
            material.color.a
        };

        glColor4fv( color );
    }
    else
    {
        glUseProgram( material.native_shader_id );
    }
}

#endif