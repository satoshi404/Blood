#pragma once

#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <renderer/gpu/core/handles.hpp>

namespace DescriptorPool
{
    bool init();
    void shutdown();

    DescriptorHandle create( const Descriptor& descriptor );
    bool update( DescriptorHandle handle, const Descriptor& descriptor );
    bool destroy( DescriptorHandle handle );

    Descriptor* get( DescriptorHandle handle );

    bool exists( DescriptorHandle handle );

    uint_32 size();
    uint_32 capacity();
}
