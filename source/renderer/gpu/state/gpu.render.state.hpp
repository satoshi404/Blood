#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>

struct GpuRenderState
{
    bool depth_test  = true_value;
    bool depth_write = true_value;
    bool cull_face   = false_value;

    bool blending = false;
    GpuBlendMode blend_mode = GpuBlendMode_Alpha;

    GpuCullMode cull_mode = GpuCullMode_Back;

    bool wireframe = false_value;

    f32 line_width = 1.0f;
    f32 point_size = 1.0f;

    bool stencil_test = false_value;
    u32 stencil_ref = 0;
};
