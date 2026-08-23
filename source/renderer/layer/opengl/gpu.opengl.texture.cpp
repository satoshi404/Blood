#include <renderer/layer/gpu.backend.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

void Backend::bind_texture( TextureHandle texture, const u32 slot )
{
    if ( !texture.is_valid() ) return;

    glActiveTexture( GL_TEXTURE0 + slot );
}

void Backend::bind_shader( ShaderHandle shader )
{
    if ( !shader.is_valid() )
        return;

    // ..
}

#endif
