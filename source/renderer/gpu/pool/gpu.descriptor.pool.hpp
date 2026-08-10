#pragma once

#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>

namespace GpuDescriptorPool
{
    bool init();
    void shutdown();

    GpuDescriptorHandle create( const GpuDescriptor& descriptor );
    bool update( GpuDescriptorHandle handle, const GpuDescriptor& descriptor );
    bool destroy( GpuDescriptorHandle handle );

    GpuDescriptor* get( GpuDescriptorHandle handle );

    bool exists( GpuDescriptorHandle handle );

    u32 size();
    u32 capacity();
}
