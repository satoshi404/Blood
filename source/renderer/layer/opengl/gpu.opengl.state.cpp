#include <renderer/layer/backend.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>
#include <vendor/libc/math.hpp>

void Backend::set_transform ( const Transform &transform )
{
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glTranslatef(
        transform.position.x,
        transform.position.y,
        transform.position.z
    );

    constexpr float_32 RAD_TO_DEG = 180.f / M_PI;
    const GLfloat rotation_x = transform.rotation.x * RAD_TO_DEG;
    const GLfloat rotation_y = transform.rotation.y * RAD_TO_DEG;
    const GLfloat rotation_z = transform.rotation.z * RAD_TO_DEG;

    glRotatef( rotation_x, 1.f, 0.f, 0.f );
    glRotatef( rotation_y, 0.f, 1.f, 0.f );
    glRotatef( rotation_z, 0.f, 0.f, 1.f );

    const GLfloat scale_x = transform.scale.x;
    const GLfloat scale_y = transform.scale.y;
    const GLfloat scale_z = transform.scale.z;
    glScalef( scale_x, scale_y, scale_z );

}

void Backend::set_render_state( const RenderState &state )
{
    if ( state.depth_test )
        glEnable( GL_DEPTH_TEST );
    else
        glDisable( GL_DEPTH_TEST );

    glDepthMask( state.depth_write ? GL_TRUE : GL_FALSE );

    if ( state.blending )
    {
        glEnable( GL_BLEND );

        switch ( state.blend_mode_type )
        {
            case BlendModeType_Additive:
                glBlendFunc( GL_SRC_ALPHA, GL_ONE );
            break;

            case BlendModeType_Multiply:
                glBlendFunc( GL_DST_COLOR, GL_ZERO );
            break;

            default:
                glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            break;
        }
    }
    else
    {
        glDisable( GL_BLEND );
    }

    if ( state.cull_face )
    {
        glEnable(GL_CULL_FACE);
        glCullFace(
            state.cull_mode_type == CullModeType_Front
                ? GL_FRONT
                : GL_BACK
            );
    }
    else
    {
        glDisable( GL_CULL_FACE );
    }

    glPolygonMode(
        GL_FRONT_AND_BACK,
        state.wireframe ? GL_LINE : GL_FILL
    );

    glLineWidth( state.line_width );
    glPointSize( state.point_size );
}

void Backend::push_state()
{
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
}

void Backend::pop_state()
{
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glPopAttrib();
}

#endif
