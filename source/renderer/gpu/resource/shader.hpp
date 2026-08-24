#pragma once

#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/core/limits.hpp>

struct Shader
{
    uint_32 native_id = 0;
    int_32  loc_mvp   = -1;
    int_32  loc_color = -1;
    char    label[ Limits::Label_Size ] = {};

    bool is_valid() const { return native_id != 0; }
};

struct ShaderDesc
{
    const char* vertex_src   = nullptr;
    const char* fragment_src = nullptr;
    const char* label        = nullptr;
};