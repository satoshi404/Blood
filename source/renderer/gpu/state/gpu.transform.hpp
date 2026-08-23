#pragma once

#include <core/types.hpp>

#include <renderer/gpu/core/enums.hpp>

struct Transform
{
    Context context;

    f32 position[3] = { 0.0f, 0.0f, 0.0f };
    f32 rotation[3] = { 0.0f, 0.0f, 0.0f };
    f32 scale[3]    = { 1.0f, 1.0f, 1.0f };

    bool absolute = false_value;
    bool matrix_dirty = true_value;
    f32 matrix[16] = {};

    void mark_dirty()
    {
        matrix_dirty = true_value;
    }
};

namespace TransformOps
{
    Transform identity();

    void set_position( Transform&, const f32 x, const f32 y, const f32 z );
    void set_rotation( Transform&, const f32 x, const f32 y, const f32 z );
    void set_scale( Transform&, const f32 x, const f32 y, const f32 z );
    void update_matrix( Transform& t );
}
