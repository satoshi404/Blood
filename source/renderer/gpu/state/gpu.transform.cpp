#include <renderer/gpu/state/gpu.transform.hpp>

GpuTransform GpuTransformOps::identity()
{
     GpuTransform t;
    // Initialize 4x4 identity matrix (flat column-major)
    t.matrix[0] = 1.0f; t.matrix[4] = 0.0f; t.matrix[8]  = 0.0f; t.matrix[12] = 0.0f;
    t.matrix[1] = 0.0f; t.matrix[5] = 1.0f; t.matrix[9]  = 0.0f; t.matrix[13] = 0.0f;
    t.matrix[2] = 0.0f; t.matrix[6] = 0.0f; t.matrix[10] = 1.0f; t.matrix[14] = 0.0f;
    t.matrix[3] = 0.0f; t.matrix[7] = 0.0f; t.matrix[11] = 0.0f; t.matrix[15] = 1.0f;
    t.matrix_dirty = false;
    return t;
}

void GpuTransformOps::set_position(GpuTransform& t, f32 x, f32 y, f32 z)
{
    t.position[0] = x;
    t.position[1] = y;
    t.position[2] = z;
    t.mark_dirty();
}

void GpuTransformOps::set_rotation(GpuTransform& t, f32 x, f32 y, f32 z)
{
    t.rotation[0] = x;
    t.rotation[1] = y;
    t.rotation[2] = z;
    t.mark_dirty();
}

void GpuTransformOps::set_scale(GpuTransform& t, f32 x, f32 y, f32 z)
{
    t.scale[0] = x;
    t.scale[1] = y;
    t.scale[2] = z;
    t.mark_dirty();
}

#include <cmath>

void GpuTransformOps::update_matrix(GpuTransform& t)
    {
        if (!t.matrix_dirty) return;

        // Precompute sines and cosines of Euler angles (assumed in radians)
        f32 cx = std::cos(t.rotation[0]);
        f32 sx = std::sin(t.rotation[0]);
        f32 cy = std::cos(t.rotation[1]);
        f32 sy = std::sin(t.rotation[1]);
        f32 cz = std::cos(t.rotation[2]);
        f32 sz = std::sin(t.rotation[2]);

        // Combined Rotation Matrix calculation using YX Z multiplication order
        // This avoids gimbal lock scenarios common in standard fly-cameras
        f32 r00 = cy * cz + sy * sx * sz;
        f32 r10 = cx * sz;
        f32 r20 = -sy * cz + cy * sx * sz;

        f32 r01 = -cy * sz + sy * sx * cz;
        f32 r11 = cx * cz;
        f32 r21 = sy * sz + cy * sx * cz;

        f32 r02 = sy * cx;
        f32 r12 = -sx;
        f32 r22 = cy * cx;

        // Inject Scale directly into the basis vectors (Column-Major SRT layout)
        t.matrix[0] = r00 * t.scale[0];
        t.matrix[1] = r10 * t.scale[0];
        t.matrix[2] = r20 * t.scale[0];
        t.matrix[3] = 0.0f;

        t.matrix[4] = r01 * t.scale[1];
        t.matrix[5] = r11 * t.scale[1];
        t.matrix[6] = r21 * t.scale[1];
        t.matrix[7] = 0.0f;

        t.matrix[8]  = r02 * t.scale[2];
        t.matrix[9]  = r12 * t.scale[2];
        t.matrix[10] = r22 * t.scale[2];
        t.matrix[11] = 0.0f;

        // Column 4: Translation (Position)
        t.matrix[12] = t.position[0];
        t.matrix[13] = t.position[1];
        t.matrix[14] = t.position[2];
        t.matrix[15] = 1.0f;

        t.matrix_dirty = false;
    }
