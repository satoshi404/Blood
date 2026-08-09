#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/layer/gpu.backend.hpp>

bool GpuBuffer::create(const f32* data, u32 count)
{
    float_count = count;
    byte_size = static_cast<usize>(count) * sizeof(f32);
    return GpuBackend::buffer_create(*this, data, count);
}

void GpuBuffer::destroy()
{
    GpuBackend::buffer_destroy(*this);
    float_count = 0;
    byte_size = 0;
}
