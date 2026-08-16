#include <renderer/layer/gpu.backend.hpp>
#include <core/debug.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

LOCAL GLenum primitive_to_gl( TopologiePrimitive primitive )
{
    switch ( primitive )
    {
        case TopologiePrimitive_Points:        return GL_POINTS;
        case TopologiePrimitive_Lines:         return GL_LINES;
        case TopologiePrimitive_LineStrip:     return GL_LINE_STRIP;
        case TopologiePrimitive_Triangles:     return GL_TRIANGLES;
        case TopologiePrimitive_TriangleStrip: return GL_TRIANGLE_STRIP;
        case TopologiePrimitive_TriangleFan:   return GL_TRIANGLE_FAN;
        default:                               return GL_TRIANGLE_STRIP;
    }
}

void GpuBackend::mesh_draw( const GpuMesh& mesh, TopologiePrimitive primitive )
{
    if (!mesh.is_valid())
    {
        Debug::Println(
            PrintColor_Red,
            "[Gpu:OpenGL] mesh invalida"
        );
        return;
    }

    glBindBuffer( GL_ARRAY_BUFFER, mesh.vertex_buffer.native_id );

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( mesh.components, GL_FLOAT, 0, nullptr );

    glDrawArrays(
        primitive_to_gl( primitive ),
        0,
        mesh.vertex_count
    );

    glDisableClientState( GL_VERTEX_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

#endif
