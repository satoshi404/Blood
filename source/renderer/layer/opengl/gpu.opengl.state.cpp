#include <renderer/layer/gpu.backend.hpp>

#if API_OPENGL

// TODO: Test
#include <GL/glew.h>
#include <GL/gl.h>

#define PI 3.14159265f

void GpuBackend::set_transform(const GpuTransform &transform)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(
        transform.position[0],
        transform.position[1],
        transform.position[2]);

    constexpr f32 RAD_TO_DEG = 180.0f / PI;
    glRotatef(transform.rotation[0] * RAD_TO_DEG, 1, 0, 0);
    glRotatef(transform.rotation[1] * RAD_TO_DEG, 0, 1, 0);
    glRotatef(transform.rotation[2] * RAD_TO_DEG, 0, 0, 1);

    glScalef(
        transform.scale[0],
        transform.scale[1],
        transform.scale[2]);
}

void GpuBackend::set_render_state(const GpuRenderState &state)
{
    if (state.depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    glDepthMask(state.depth_write ? GL_TRUE : GL_FALSE);

    if (state.blending)
    {
        glEnable(GL_BLEND);

        switch (state.blend_mode)
        {
        case GpuBlendMode_Additive:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        case GpuBlendMode_Multiply:
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            break;

        default:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
    }
    else
    {
        glDisable(GL_BLEND);
    }

    if (state.cull_face)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(
            state.cull_mode == GpuCullMode_Front
                ? GL_FRONT
                : GL_BACK);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(
        GL_FRONT_AND_BACK,
        state.wireframe ? GL_LINE : GL_FILL);

    glLineWidth(state.line_width);
    glPointSize(state.point_size);
}

void GpuBackend::push_state()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void GpuBackend::pop_state()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}

#endif
