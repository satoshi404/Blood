#pragma once

#include <renderer/layer/backend.hpp>

#include <renderer/gpu/core/types.hpp>

struct Material
{
    Color color = {};
    ShaderHandle shader = {};
    TextureHandle albedo = {};

    float_32 metallic = 0.0f;
    float_32 roughness = 1.0f;

    // Compatibility/native shader identifier for the current OpenGL path.
    uint_32 native_shader_id = 0;

    void bind() const
    {
        Backend::bind_material( this );
    };
};
