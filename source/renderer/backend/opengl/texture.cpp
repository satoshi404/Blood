#include <renderer/backend/backend.hpp>

#if API_OPENGL

#include <renderer/backend/opengl/opengl.hpp>

void Backend::bind_texture( TextureHandle handle, const uint_32 slot )
{
    Texture* texture = Pool::get_texture( handle );
    if ( !texture->is_valid() ) return;

    glActiveTexture( GL_TEXTURE0 + slot );
}


#endif
