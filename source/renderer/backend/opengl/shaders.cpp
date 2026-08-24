#include <renderer/backend/backend.hpp>

#include <core/debug.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>
#include <core/debug.hpp>
#include <cstring>

namespace
{
    uint_32 compile_stage( uint_32 type, const char* src )
    {
        uint_32 id = glCreateShader( type );
        glShaderSource( id, 1, &src, nullptr );
        glCompileShader( id );

        int_32 ok = 0;
        glGetShaderiv( id, GL_COMPILE_STATUS, &ok );
        if ( !ok )
        {
            char log[ 512 ];
            glGetShaderInfoLog( id, 512, nullptr, log );
            Debug::Println( PrintColorType_Red, "[Shader] compile: %s", log );
            glDeleteShader( id );
            return 0;
        }
        return id;
    }
}

bool Backend::shader_create_from_source(
    Shader& out,
    const char* vertex_src,
    const char* fragment_src )
{
    out = {};
    if ( !vertex_src || !fragment_src )
        return false_value;

    uint_32 vs = compile_stage( GL_VERTEX_SHADER, vertex_src );
    uint_32 fs = compile_stage( GL_FRAGMENT_SHADER, fragment_src );
    if ( !vs || !fs )
    {
        if ( vs ) glDeleteShader( vs );
        if ( fs ) glDeleteShader( fs );
        return false_value;
    }

    uint_32 prog = glCreateProgram();
    glAttachShader( prog, vs );
    glAttachShader( prog, fs );
    glLinkProgram( prog );

    glDeleteShader( vs );
    glDeleteShader( fs );

    int_32 linked = 0;
    glGetProgramiv( prog, GL_LINK_STATUS, &linked );
    if ( !linked )
    {
        char log[ 512 ];
        glGetProgramInfoLog( prog, 512, nullptr, log );
        Debug::Println( PrintColorType_Red, "[Shader] link: %s", log );
        glDeleteProgram( prog );
        return false_value;
    }

    out.native_id = prog;
    out.loc_mvp   = glGetUniformLocation( prog, "u_mvp" );
    out.loc_color = glGetUniformLocation( prog, "u_color" );
    return true_value;
}

void Backend::shader_destroy( Shader& shader )
{
    if ( shader.native_id )
    {
        glDeleteProgram( shader.native_id );
        shader.native_id = 0;
    }
    shader.loc_mvp = shader.loc_color = -1;
}

void Backend::bind_shader( const Shader& shader )
{
    if ( shader.native_id )
        glUseProgram( shader.native_id );
    else
        glUseProgram( 0 );
}

void Backend::set_shader_color( const Shader& shader, const Color& color )
{
    if ( shader.loc_color >= 0 )
        glUniform4f( shader.loc_color, color.r, color.g, color.b, color.a );
}

#endif