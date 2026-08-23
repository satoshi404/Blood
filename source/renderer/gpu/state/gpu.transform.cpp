#include <renderer/gpu/state/gpu.transform.hpp>

#include <vendor/libc/math.hpp>

Transform TransformOps::identity()
{
    Transform t;

    t.matrix[0] = 1.f; t.matrix[4] = 0.f; t.matrix[8]  = 0.f; t.matrix[12] = 0.f;
    t.matrix[1] = 0.f; t.matrix[5] = 1.f; t.matrix[9]  = 0.f; t.matrix[13] = 0.f;
    t.matrix[2] = 0.f; t.matrix[6] = 0.f; t.matrix[10] = 1.f; t.matrix[14] = 0.f;
    t.matrix[3] = 0.f; t.matrix[7] = 0.f; t.matrix[11] = 0.f; t.matrix[15] = 1.f;
    t.matrix_dirty = false;
    return t;
}

void TransformOps::set_position( Transform& t, const f32 x, const f32 y, const f32 z )
{
    t.position[0] = x;
    t.position[1] = y;
    t.position[2] = z;
    t.mark_dirty();
}

void TransformOps::set_rotation( Transform& t, const f32 x, const f32 y, const f32 z )
{
    t.rotation[0] = x;
    t.rotation[1] = y;
    t.rotation[2] = z;
    t.mark_dirty();
}

void TransformOps::set_scale( Transform& t, const f32 x, const f32 y, const f32 z )
{
    t.scale[0] = x;
    t.scale[1] = y;
    t.scale[2] = z;
    t.mark_dirty();
}

void TransformOps::update_matrix( Transform& t )
    {
        if (!t.matrix_dirty) return;

        // Rotation
        const f32 cx = cos( t.rotation[0] );
        const f32 sx = sin( t.rotation[0] );
        const f32 cy = cos( t.rotation[1] );
        const f32 sy = sin( t.rotation[1] );
        const f32 cz = cos( t.rotation[2] );
        const f32 sz = sin( t.rotation[2] );

        const f32 r00 = cy * cz + sy * sx * sz;
        const f32 r10 = cx * sz;
        const f32 r20 = -sy * cz + cy * sx * sz;

        const f32 r01 = -cy * sz + sy * sx * cz;
        const f32 r11 = cx * cz;
        const f32 r21 = sy * sz + cy * sx * cz;

        const f32 r02 = sy * cx;
        const f32 r12 = -sx;
        const f32 r22 = cy * cx;

        // Scale
        t.matrix[0] = r00 * t.scale[0];
        t.matrix[1] = r10 * t.scale[0];
        t.matrix[2] = r20 * t.scale[0];
        t.matrix[3] = 0.f;

        t.matrix[4] = r01 * t.scale[1];
        t.matrix[5] = r11 * t.scale[1];
        t.matrix[6] = r21 * t.scale[1];
        t.matrix[7] = 0.f;

        t.matrix[8]  = r02 * t.scale[2];
        t.matrix[9]  = r12 * t.scale[2];
        t.matrix[10] = r22 * t.scale[2];
        t.matrix[11] = 0.f;

        // Translate
        t.matrix[12] = t.position[0];
        t.matrix[13] = t.position[1];
        t.matrix[14] = t.position[2];
        t.matrix[15] = 1.f;

        t.matrix_dirty = false_value;
    }
