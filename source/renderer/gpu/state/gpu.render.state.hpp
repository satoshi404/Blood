#pragma once

#include <renderer/gpu/core/enums.hpp>

struct RenderState
{
    bool depth_test  = true_value;
    bool depth_write = true_value;
    bool cull_face   = false_value;

    bool blending = false_value;
    BlendModeType blend_mode_type = BlendModeType_Alpha;

    CullModeType cull_mode_type = CullModeType_Back;

    bool wireframe = false_value;

    float_32 line_width = 1.f;
    float_32 point_size = 1.f;

    bool stencil_test = false_value;
    uint_32 stencil_ref = 0;
};
