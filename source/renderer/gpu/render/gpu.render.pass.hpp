#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>
#include <renderer/gpu/render/gpu.render.queue.hpp>
#include <renderer/gpu/core/gpu.types.hpp>

struct GpuRenderPass
{
    //GpuRenderSpace space;

    GpuViewport viewport;

    GpuRenderQueue* queue;

    bool clear_enabled;
};