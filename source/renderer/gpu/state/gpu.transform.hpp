#pragma once

#include <core/types.hpp>


struct GpuTransform
{
    f32 position[3] = { 0.0f, 0.0f, 0.0f };
    f32 rotation[3] = { 0.0f, 0.0f, 0.0f };
    f32 scale[3]    = { 1.0f, 1.0f, 1.0f };

    bool absolute = false;
    bool matrix_dirty = true;
    f32 matrix[16] = {};

    void mark_dirty()
    {
        matrix_dirty = true;
    }
};

namespace GpuTransformOps
{
    GpuTransform identity();

    void set_position(GpuTransform&, f32 x, f32 y, f32 z);
    void set_rotation(GpuTransform&, f32 x, f32 y, f32 z);
    void set_scale(GpuTransform&, f32 x, f32 y, f32 z);
    void update_matrix(GpuTransform& t);
}
