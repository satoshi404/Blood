#include <renderer/layer/gpu.backend.hpp>

#if API_OPENGL

// TODO: Test
#include <GL/glew.h>
#include <GL/gl.h>

//#include <renderer/opengl/gl.hpp>

void GpuBackend::bind_texture(GpuTextureHandle texture, u32 slot)
{
    if (!texture.is_valid())
        return;

    glActiveTexture(GL_TEXTURE0 + slot);
    // The resource pool will eventually resolve the engine handle
    // into the native OpenGL texture object.
    // Kept intentionally as a no-op until GpuTexturePool owns creation.
}

void GpuBackend::bind_shader(GpuShaderHandle shader)
{
    if (!shader.is_valid())
        return;
}

#endif
