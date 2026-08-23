#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/layer/backend.hpp>

bool Buffer::create(const float_32* data, uint_32 count)
{
    float_count = count;
    byte_size = static_cast<unsigned_size>(count) * sizeof(float_32);
    return Backend::buffer_create(*this, data, count);
}

void Buffer::destroy()
{
    Backend::buffer_destroy(*this);
    float_count = 0;
    byte_size = 0;
}
