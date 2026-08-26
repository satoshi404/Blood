#include <renderer/backend/backend.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

bool Backend::buffer_create( Buffer& buffer, const float_32* data, const uint_32 count )
{
    glGenBuffers( 1, &buffer.native_id );
    glBindBuffer( GL_ARRAY_BUFFER, buffer.native_id );
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>( count * sizeof(float_32) ),
        data,
        GL_STATIC_DRAW
    );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    buffer.float_count = count;
    buffer.byte_size = static_cast<unsigned_size>( count ) * sizeof( float_32 );

    return buffer.native_id != 0;
}

void Backend::buffer_destroy( Buffer& buffer )
{
    if (!buffer.native_id)
        return;

    glDeleteBuffers(1, &buffer.native_id);
    buffer.native_id = 0;
}

#endif
