#include <renderer/gpu/state/transform.hpp>

#include <vendor/libc/math.hpp>

Transform TransformOps::identity()
{
    static Transform trasform;
    trasform.matrix = mat4_float_vec4_identity;
    trasform.matrix_dirty = false;
    return trasform;
}

void TransformOps::set_position( Transform& trasform, const float_32 x, const float_32 y, const float_32 z )
{
    trasform.position = fvec3( x, y, z );
    trasform.mark_dirty();
}

void TransformOps::set_rotation( Transform& trasform, const float_32 x, const float_32 y, const float_32 z )
{
    trasform.rotation = fvec3( x, y, z );
    trasform.mark_dirty();
}

void TransformOps::set_scale( Transform& trasform, const float_32 x, const float_32 y, const float_32 z )
{
    trasform.scale = fvec3( x, y, z );
    trasform.mark_dirty();
}

void TransformOps::update_matrix( Transform& transform )
    {
        if (!transform.matrix_dirty) return;

        const float_32 cx = cos( transform.rotation.x );
        const float_32 sx = sin( transform.rotation.x );
        const float_32 cy = cos( transform.rotation.y );
        const float_32 sy = sin( transform.rotation.y );
        const float_32 cz = cos( transform.rotation.z );
        const float_32 sz = sin( transform.rotation.z );

        const float_32 r00 = cy * cz + sy * sx * sz;
        const float_32 r10 = cx * sz;
        const float_32 r20 = -sy * cz + cy * sx * sz;

        const float_32 r01 = -cy * sz + sy * sx * cz;
        const float_32 r11 = cx * cz;
        const float_32 r21 = sy * sz + cy * sx * cz;

        const float_32 r02 = sy * cx;
        const float_32 r12 = -sx;
        const float_32 r22 = cy * cx;

        transform.matrix = mat4_fvec4(
            fvec4( r00 * transform.scale.x, r10 * transform.scale.x, r20 * transform.scale.x, 0.f ),
            fvec4( r01 * transform.scale.y, r11 * transform.scale.y, r21 * transform.scale.y, 0.f ),
            fvec4( r02 * transform.scale.z, r12 * transform.scale.z, r22 * transform.scale.z, 0.f ),
            fvec4( transform.position.x, transform.position.y, transform.position.z,  1.f )
        );
        transform.matrix_dirty = false_value;
    }
