#pragma once

#include <renderer/gpu/core/enums.hpp>

struct RenderState
{
    bool depth_test  = true_value;
    bool depth_write = true_value;
    bool cull_face   = false_value;

    bool blending = false;
    BlendMode blend_mode = BlendModeType_Alpha;

    CullMode cull_mode = CullModeType_Back;

    bool wireframe = false_value;

    f32 line_width = 1.f;
    f32 point_size = 1.f;

    bool stencil_test = false_value;
    u32 stencil_ref = 0;
};
