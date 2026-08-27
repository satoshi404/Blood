#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/pool.hpp>
#include <renderer/backend/backend.hpp>

#include <core/debug.hpp>


// shader unlit embutido ou carregado de arquivo depois
static const char* VS = R"(#version 330 core
layout(location=0) in vec3 a_pos;
uniform mat4 u_mvp;
void main(){ gl_Position = u_mvp * vec4(a_pos,1.0); }
)";

static const char* FS = R"(#version 330 core
uniform vec4 u_color;
out vec4 frag;
void main(){ frag = u_color; }
)";

void Material::bind() const
{
    if ( shader.is_valid() )
    {
        Shader* s = Pool::get_shader( shader );
        if ( s->is_valid() )
        {
            Debug::Print( PrintColorType_Green, "Shader" );
            Backend::bind_shader( *s );
            Backend::set_shader_color( *s, BlueColor );
            return;
        }
    }

    // fallback fixed-function (caminho atual do cubo)
    Backend::bind_material( *this ); // glColor4fv
}