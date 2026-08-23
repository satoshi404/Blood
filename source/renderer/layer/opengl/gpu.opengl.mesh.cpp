#include <renderer/layer/backend.hpp>
#include <core/debug.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

LOCAL GLenum primitive_to_gl( TopologiePrimitiveType primitive_type )
{
    switch ( primitive_type )
    {
        case TopologiePrimitiveType_Points:        return GL_POINTS;
        case TopologiePrimitiveType_Lines:         return GL_LINES;
        case TopologiePrimitiveType_LineStrip:     return GL_LINE_STRIP;
        case TopologiePrimitiveType_Triangles:     return GL_TRIANGLES;
        case TopologiePrimitiveType_TriangleStrip: return GL_TRIANGLE_STRIP;
        case TopologiePrimitiveType_TriangleFan:   return GL_TRIANGLE_FAN;
        default:                                   return GL_TRIANGLE_STRIP;
    }
}

void Backend::mesh_draw( const Mesh& mesh, TopologiePrimitiveType primitive_type )
{
    if ( !mesh.is_valid() )
    {
        Debug::Println(
            PrintColorType_Red,
            "[Gpu:OpenGL] mesh invalida"
        );
        return;
    }

    glBindBuffer( GL_ARRAY_BUFFER, mesh.vertex_buffer.native_id );

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( mesh.components, GL_FLOAT, 0, nullptr );

    glDrawArrays(
        primitive_to_gl( primitive_type ),
        0,
        mesh.vertex_count
    );

    glDisableClientState( GL_VERTEX_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

#endif
