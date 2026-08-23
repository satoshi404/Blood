#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/render/gpu.render.queue.hpp>
#include <renderer/gpu/core/types.hpp>

enum_type( LoadOpType, uint_8 )
{
    LoadOpType_Clear,
    LoadOpType_Load,
    LoadOpType_DontCare
};

enum_type( StoreOpType, uint_8 )
{
    StoreOpType_Store,
    StoreOpType_DontCare
};

struct AttachmentDesc
{
    TextureHandle texture = {};
    LoadOpType load_op = LoadOpType_Clear;
    StoreOpType store_op = StoreOpType_Store;
    Color clear = {};
};

struct RenderPass
{
    char label[ Limits::Label_Size ] = {};
    uint_32 id = 0;

    // Viewport / Scissor
    Viewport viewport = {};
    bool has_scissor = false_value;

    // TODO: Implementation
    // GpuScissor scissor;

    // TODO: Basic color + depth yet
    AttachmentDesc color;
    AttachmentDesc depth;

    RenderQueueHandle queue = {};

    ContextType context_type = ContextType_2D; // Default

    // Flags
    bool clear_enabled = true_value;
    bool enabled = true_value;
};