#include <renderer/layer/gpu.backend.hpp>

#if API_OPENGL


// TODO: Test
#include <GL/glew.h>
#include <GL/gl.h>


void GpuBackend::material_bind(const GpuMaterial& material)
{
    if (material.native_shader_id == 0)
    {
        const f32 color[4] =
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
