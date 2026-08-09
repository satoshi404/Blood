#pragma once

#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/core/gpu.types.hpp>

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
