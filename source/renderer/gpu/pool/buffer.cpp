#include <renderer/gpu/pool/pool.hpp>
#include <renderer/gpu/core/limits.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/gpu.buffer.hpp>

static Slot<Buffer> slots[ GpuLimits::MaxBuffers ] = {};

BufferHandle Pool::create_buffer( const f32* data, u32 float_count )
{
	// ..
}

void Pool::free()
{
	// ..
}
