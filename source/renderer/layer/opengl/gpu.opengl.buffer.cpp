#include <renderer/layer/gpu.backend.hpp>

#if API_OPENGL

// TODO: Test
#include <GL/glew.h>
#include <GL/gl.h>

bool GpuBackend::buffer_create(GpuBuffer& buffer, const f32* data, u32 float_count)
{
    glGenBuffers(1, &buffer.native_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.native_id);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(float_count * sizeof(f32)),
        data,
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    buffer.float_count = float_count;
    buffer.byte_size = static_cast<usize>(float_count) * sizeof(f32);

    return buffer.native_id != 0;
}

void GpuBackend::buffer_destroy(GpuBuffer& buffer)
{
    if (!buffer.native_id)
        return;

    glDeleteBuffers(1, &buffer.native_id);
    buffer.native_id = 0;
}

#endif
