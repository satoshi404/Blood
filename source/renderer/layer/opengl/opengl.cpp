#include <renderer/layer/backend.hpp>
#include <core/debug.hpp>
#include <platform/window.hpp>

#include <core/math.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

// TODO:
#include <vendor/libc/math.hpp>

bool Backend::init()
{
    GLenum status = glewInit();

    if (status != GLEW_OK)
    {
        Debug::Println(
            PrintColorType_Red,
            "[Gpu:OpenGL] glewInit falhou: %s",
            glewGetErrorString(status));
        return false_value;
    }

    glEnable(GL_DEPTH_TEST);
    return true_value;
}

void Backend::shutdown()
{
}

void Backend::clear( Color color )
{
    color.normalized();
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Backend::viewport(const Viewport &vp)
{
    glViewport(vp.x, vp.y, vp.width, vp.height);

    const float_32 aspect = (float_32)vp.width / (float_32)vp.height;
    const float_32 fov_y   = 60.0f;   // graus
    const float_32 z_near  = 0.1f;
    const float_32 z_far   = 1000.0f;

    const float_32 far = 1.0f / tanf(fov_y * 0.5f * ( RAD_180 ));

    mat4_float_vec4 projection = mat4_float_vec4_zero;

    projection = mat4_fvec4(
        fvec4( far / aspect, 0.0f, 0.0f, 0.0f ),
        fvec4( 0.0f,       far,    0.0f,                                 0.0f ),
        fvec4( 0.0f,       0.0f, (z_far + z_near) / (z_near - z_far),  -1.0f ),
        fvec4( 0.0f,       0.0f, (2.0f * z_far * z_near) / (z_near - z_far), 0.0 )
    );

    glMatrixMode(GL_PROJECTION);

    const float_32 projection_repr[16] =
    {
        projection.x.x, projection.x.y, projection.x.z, projection.x.w,
        projection.y.x, projection.y.y, projection.y.z, projection.y.w,
        projection.z.x, projection.z.y, projection.z.z, projection.z.w,
        projection.w.x, projection.w.y, projection.w.z, projection.w.w,
    };

    glLoadMatrixf( projection_repr );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Backend::swap()
{
    CoreWindow::swap_buffers();
}

#endif
