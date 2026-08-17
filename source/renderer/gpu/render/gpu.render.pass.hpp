#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>
#include <renderer/gpu/core/gpu.limits.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/render/gpu.render.queue.hpp>
#include <renderer/gpu/core/gpu.types.hpp>

enum_type( GpuLoadOp, u8 )
{
    GpuLoadOp_Clear,
    GpuLoadOp_Load,
    GpuLoadOp_DontCare
};

enum_type( GpuStoreOp, u8 )
{
    GpuStoreOp_Store,
    GpuStoreOp_DontCare
};

struct GpuAttachmentDesc
{
    GpuTextureHandle texture = {};
    GpuLoadOp load_op = GpuLoadOp_Clear;
    GpuStoreOp store_op = GpuStoreOp_Store;
    GpuClear clear = {};
};

struct GpuRenderPass
{
    char label[GpuLimits::LabelSize] = {};
    u32 id = 0;

    // Viewport / Scissor
    GpuViewport viewport = {};
    bool has_scissor = false_value;

    // TODO: Implementation
    // GpuScissor scissor;

    // TODO: Basic color + depth yet
    GpuAttachmentDesc color;
    GpuAttachmentDesc depth;

    GpuRenderQueueHandle queue = {};

    GpuContext context = GpuContext_2D; // Default

    // Flags
    bool clear_enabled = true_value;
    bool enabled = true_value;
};