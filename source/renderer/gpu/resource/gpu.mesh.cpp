#include <renderer/gpu/resource/gpu.mesh.hpp>
#include <renderer/layer/gpu.backend.hpp>

bool GpuMesh::upload(const f32* positions, u32 count, u32 comps)
{
    if (!positions || count == 0 || comps == 0)
        return false_value;

    components = comps;
    vertex_count = count;

    if (!vertex_buffer.create(positions, count * comps))
    {
        vertex_count = 0;
        return false_value;
    }

    return true_value;
}

void GpuMesh::draw(GpuPrimitive primitive_override) const
{
    const GpuPrimitive p =
        primitive_override != GpuPrimitive_Default
        ? primitive_override
        : primitive;

    GpuBackend::mesh_draw(*this, p);
}

void GpuMesh::destroy()
{
    vertex_buffer.destroy();
    vertex_count = 0;
}
