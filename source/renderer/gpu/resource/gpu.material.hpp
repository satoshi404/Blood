#pragma once

#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/core/types.hpp>

struct GpuMaterial
{
    GpuColor color = {};
    GpuShaderHandle shader = {};
    GpuTextureHandle albedo = {};

    f32 metallic = 0.0f;
    f32 roughness = 1.0f;

    // Compatibility/native shader identifier for the current OpenGL path.
    u32 native_shader_id = 0;

    void bind() const;
};
