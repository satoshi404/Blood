#include <renderer/gpu/resource/gpu.material.hpp>
#include <renderer/layer/gpu.backend.hpp>

void GpuMaterial::bind() const
{
    GpuBackend::material_bind(*this);
}
