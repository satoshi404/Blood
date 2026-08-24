#pragma once

#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/material.hpp>

namespace MaterialPool
{
    bool init();
    void shutdown();

    MaterialHandle create( const Material& material );
    bool           update( MaterialHandle handle, const Material& material );
    bool           destroy( MaterialHandle handle );

    Material*       get( MaterialHandle handle );
    const Material* get_const( MaterialHandle handle );

    bool    exists( MaterialHandle handle );
    uint_32 size();
    uint_32 capacity();
}