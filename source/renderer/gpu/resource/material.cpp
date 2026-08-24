#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/pool/shader.hpp>
#include <renderer/backend/backend.hpp>

#include <renderer/gpu/pool/shader.hpp>

void Material::bind() const
{
    if ( shader.is_valid() )
    {
        const Shader* s = ShaderPool::get_const( shader );
        if ( s && s->is_valid() )
        {
            Backend::bind_shader( *s );
            Backend::set_shader_color( *s, color );
            return;
        }
    }

    // fallback fixed-function (caminho atual do cubo)
    Backend::bind_material( *this ); // glColor4fv
}