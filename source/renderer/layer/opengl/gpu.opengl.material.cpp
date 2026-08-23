#include <renderer/layer/backend.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

void Backend::bind_material( Material material )
{
    if (material.native_shader_id == 0)
    {
        const float_32 color[4] =
        {
            material.color.r,
            material.color.g,
            material.color.b,
            material.color.a
        };

        glColor4fv(color);
    }
    else
    {
        glUseProgram(material.native_shader_id);
    }
}

#endif