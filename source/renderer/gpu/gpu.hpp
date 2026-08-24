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

    extern DescriptorHandle create_descriptor( const Descriptor& descriptor );
    extern bool update_descriptor( DescriptorHandle handle, const Descriptor& descriptor );
    extern bool destroy_descriptor( DescriptorHandle handle );
    extern bool destroy_pool( BufferHandle handle );

    extern MaterialHandle create_material( const Material& material );
    extern bool           update_material( MaterialHandle handle, const Material& material );
    extern bool           destroy_material( MaterialHandle handle );
    extern Material*      get_material( MaterialHandle handle );
    extern const Material* get_material_const( MaterialHandle handle );

    ShaderHandle create_shader( const char* vs, const char* fs, const char* label = nullptr );
    bool         destroy_shader( ShaderHandle handle );
    Shader*      get_shader( ShaderHandle handle );

    extern const Descriptor* get_descriptor( DescriptorHandle handle );
    extern Descriptor* get_descriptor_mutable( DescriptorHandle handle );

    extern void submit( CommandList& command_list );
    extern void execute( const Command&  command_list );
}
