#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>

struct GpuRenderState
{
    bool depth_test  = true;
    bool depth_write = true;
    bool cull_face   = true;

    bool blending = false;
    GpuBlendMode blend_mode = GpuBlendMode_Alpha;

    bool cull_face = false;
    GpuCullMode cull_mode = GpuCullMode_Back;

    bool wireframe = false;

    f32 line_width = 1.0f;
    f32 point_size = 1.0f;

    bool stencil_test = false;
    u32 stencil_ref = 0;
};
