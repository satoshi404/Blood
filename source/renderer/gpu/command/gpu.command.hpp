#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/core/gpu.types.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/resource/gpu.material.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>

struct GpuDrawCommand
{
    GpuDescriptorHandle descriptor = {};
};

struct GpuMaterialCommand
{
    const GpuMaterial* material = nullptr;
    GpuShaderHandle shader = {};
    bool override_material = false;
};

struct GpuTextureCommand
{
    GpuTextureHandle texture = {};
    u32 slot = 0;
};

struct GpuCommand
{
    GpuCommandType type = GpuCommandType_Clear;

    union
    {
        GpuClear clear;
        GpuDrawCommand draw;
        GpuViewport viewport;
        GpuTransform transform;
        GpuMaterialCommand material;
        GpuTextureCommand texture;
        GpuRenderState render_state;
    } commands;

    char label[GpuLimits::LabelSize] = {};
    u64 timestamp = 0;
    bool enabled = true;

    static GpuCommand clear(
        f32 r,
        f32 g,
        f32 b,
        f32 a = 1.0f,
        const char* label = nullptr
    );

    static GpuCommand draw(
        GpuDescriptorHandle descriptor,
        const char* label = nullptr
    );

    static GpuCommand viewport(
        i32 x,
        i32 y,
        i32 width,
        i32 height,
        const char* label = nullptr
    );

    static GpuCommand swap(
        const char* label = nullptr
    );

    static GpuCommand transform(
        const GpuTransform& value,
        const char* label = nullptr
    );

    static GpuCommand material(
        const GpuMaterial* value,
        const char* label = nullptr
    );

    static GpuCommand render_state(
        const GpuRenderState& value,
        const char* label = nullptr
    );

    static GpuCommand texture(
        GpuTextureHandle texture,
        u32 slot,
        const char* label = nullptr
    );

    static GpuCommand push_state(
        const char* label = nullptr
    );

    static GpuCommand pop_state(
        const char* label = nullptr
    );
};
