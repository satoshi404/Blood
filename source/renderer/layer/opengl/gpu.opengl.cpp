#include <renderer/layer/gpu.backend.hpp>
#include <core/debug.hpp>
#include <platform/window.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

// TODO:
#include <vendor/libc/math.hpp>

bool GpuBackend::init()
{
    GLenum status = glewInit();

    if (status != GLEW_OK)
    {
        Debug::Println(
            PrintColor_Red,
            "[Gpu:OpenGL] glewInit falhou: %s",
            glewGetErrorString(status));
        return false_value;
    }

    glEnable(GL_DEPTH_TEST);
    return true_value;
}

void GpuBackend::shutdown()
{
}

void GpuBackend::clear(const GpuClear &c)
{
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GpuBackend::viewport(const GpuViewport &vp)
{
    glViewport(vp.x, vp.y, vp.width, vp.height);

    const f32 aspect = (f32)vp.width / (f32)vp.height;
    const f32 fov_y   = 60.0f;   // graus
    const f32 z_near  = 0.1f;
    const f32 z_far   = 1000.0f;

    const f32 f = 1.0f / tanf(fov_y * 0.5f * (3.14159265f / 180.0f));

    const f32 proj[16] =
    {
        f / aspect, 0.0f, 0.0f,                                 0.0f,
        0.0f,       f,    0.0f,                                 0.0f,
        0.0f,       0.0f, (z_far + z_near) / (z_near - z_far),  -1.0f,
        0.0f,       0.0f, (2.0f * z_far * z_near) / (z_near - z_far), 0.0f
    };

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(proj);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GpuBackend::swap()
{
    CoreWindow::swap_buffers();
}

#endif
