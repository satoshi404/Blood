#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/types.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <renderer/gpu/resource/gpu.buffer.hpp>
#include <renderer/gpu/resource/gpu.mesh.hpp>
#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/resource/gpu.texture.hpp>
#include <renderer/gpu/resource/gpu.shader.hpp>
#include <renderer/gpu/state/gpu.transform.hpp>
#include <renderer/gpu/state/gpu.render.state.hpp>
#include <renderer/gpu/command/gpu.command.hpp>
#include <renderer/gpu/command/gpu.command.list.hpp>

namespace Gpu
{
    extern bool init();
    extern void shutdown();

    extern void new_frame();
    extern const Statistics& statistics();

    extern DescriptorHandle create_descriptor( const Descriptor& descriptor );
    extern bool update_descriptor( DescriptorHandle handle, const Descriptor& descriptor );
    extern bool destroy_descriptor( DescriptorHandle handle );

    extern const Descriptor* get_descriptor( DescriptorHandle handle );
    extern Descriptor* get_descriptor_mutable( DescriptorHandle handle );

    extern void submit( CommandList& command_list );
    extern void execute( const Command&  command_list );
}
