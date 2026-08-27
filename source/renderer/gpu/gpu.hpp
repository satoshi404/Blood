#pragma once

#include <renderer/gpu/core/enums.hpp>
#include <renderer/gpu/core/types.hpp>
#include <renderer/gpu/core/handles.hpp>
#include <renderer/gpu/resource/descriptor.hpp>
#include <renderer/gpu/resource/buffer.hpp>
#include <renderer/gpu/resource/mesh.hpp>
#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/resource/texture.hpp>
#include <renderer/gpu/resource/shader.hpp>
#include <renderer/gpu/state/transform.hpp>
#include <renderer/gpu/state/state.hpp>
#include <renderer/gpu/command/command.hpp>
#include <renderer/gpu/command/command.list.hpp>

namespace Gpu
{
    extern bool init();
    extern void shutdown();

    extern void new_frame();
    extern const Statistics& statistics();

    extern void submit( CommandList& command_list );
    extern void execute( const Command&  command_list );
}
