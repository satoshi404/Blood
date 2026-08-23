#pragma once

#include <renderer/gpu/core/gpu.enums.hpp>
#include <renderer/gpu/core/gpu.types.hpp>
#include <renderer/gpu/core/gpu.handles.hpp>
#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/gpu/resource/gpu.mesh.hpp>
#include <renderer/gpu/resource/gpu.material.hpp>
#include <renderer/gpu/resource/gpu.texture.hpp>
#include <renderer/gpu/resource/gpu.shader.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/command/gpu.command.hpp>
#include <renderer/gpu/command/gpu.command.list.hpp>

namespace Gpu
{
    bool init();
    void shutdown();

    void new_frame();
    const GpuStatistics& statistics();

    GpuDescriptorHandle create_descriptor (const GpuDescriptor& );
    bool update_descriptor( GpuDescriptorHandle, const GpuDescriptor& );
    bool destroy_descriptor( GpuDescriptorHandle );

    const GpuDescriptor* get_descriptor( GpuDescriptorHandle );
    GpuDescriptor* get_descriptor_mutable( GpuDescriptorHandle );

    void submit( GpuCommandList& );
    void execute( const GpuCommand& );
}
