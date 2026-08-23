#pragma once

#include <renderer/gpu/core/handles.hpp>

struct Texture
{
    uint_32 native_id = 0;
    uint_32 width = 0;
    uint_32 height = 0;
    uint_32 channels = 0;

    bool is_valid() const
    {
        return native_id != 0;
    }
};
