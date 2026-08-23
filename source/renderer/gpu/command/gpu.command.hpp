#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/core/types.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/render/gpu.render.pass.hpp>
#include <renderer/gpu/render/gpu.render.queue.hpp>
#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/core/limits.hpp>

struct BeginRenderPassCommand
{
    RenderPass pass;
};

struct ExecuteQueueCommand
{
    RenderQueueHandle queue;
};

struct DrawCommand
{
    DescriptorHandle descriptor = {};
};

struct MaterialCommand
{
    const Material* material = nullptr;
    ShaderHandle shader = {};
    bool override_material = false_value;
};

struct TextureCommand
{
    TextureHandle texture = {};
    uint_32 slot = 0;
};

struct Command
{

    CommandType type = CommandType_Clear;

    struct
    {
        Color clear;
        DrawCommand draw;
        Viewport viewport;
        Transform transform;
        MaterialCommand material;
        TextureCommand texture;
        RenderState render_state;
        BeginRenderPassCommand begin_pass;
        ExecuteQueueCommand execute_pass;
    } commands;

    char label[ Limits::Label_Size ] = {};
    uint_64 timestamp = 0;
    bool enabled = true_value;

    static Command clear(
        Color color = DefaultClearColor,
        const char* label = nullptr
    );

    static Command draw(
        DescriptorHandle descriptor,
        const char* label = nullptr
    );

    static Command viewport(
        const int_32 x,
        const int_32 y,
        const int_32 width,
        const int_32 height,
        const char* label = nullptr
    );

    static Command swap(
        const char* label = nullptr
    );

    static Command transform(
        const Transform& value,
        const char* label = nullptr
    );

    static Command material(
        const Material* value,
        const char* label = nullptr
    );

    static Command render_state(
        const RenderState& value,
        const char* label = nullptr
    );

    static Command texture(
        TextureHandle texture,
        const uint_32 slot,
        const char* label = nullptr
    );

    static Command push_state(
        const char* label = nullptr
    );

    static Command pop_state(
        const char* label = nullptr
    );

    static Command begin_render_pass( const RenderPass& pass, const char* label = nullptr );
    static Command end_render_pass( const char* label = nullptr );
    static Command execute_queue( const RenderQueueHandle& queue, const char* label = nullptr );
};
