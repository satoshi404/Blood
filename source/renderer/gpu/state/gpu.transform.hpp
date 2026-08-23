#pragma once

#include <core/types.hpp>
#include <core/math.hpp>

#include <renderer/gpu/core/enums.hpp>

struct Transform
{
    ContextType context_type;

    float_vec3 position = fvec3_zero;
    float_vec3 rotation = fvec3_zero;
    float_vec3 scale    = fvec3( 1.f, 1.f, 1.f );

    bool absolute = false_value;
    bool matrix_dirty = true_value;
    mat4_float_vec4 matrix = mat4_float_vec4_zero;

    void mark_dirty()
    {
        matrix_dirty = true_value;
    }
};

namespace TransformOps
{
    Transform identity();

    void set_position( Transform& trasform, const float_32 x, const float_32 y, const float_32 z );
    void set_rotation( Transform& trasform, const float_32 x, const float_32 y, const float_32 z );
    void set_scale( Transform& trasform, const float_32 x, const float_32 y, const float_32 z );
    void update_matrix( Transform& trensform );
}
