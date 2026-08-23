#pragma once

#include <renderer/gpu/core/handles.hpp>

struct Shader
{
    uint_32 native_id = 0;

    bool is_valid() const
    {
        return native_id != 0;
    }
};
