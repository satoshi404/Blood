#include <renderer/gpu/gpu.hpp>

#include <renderer/backend/backend.hpp>
#include <renderer/gpu/pool/descriptor.hpp>
#include <renderer/gpu/command/command.dispatch.hpp>
#include <renderer/gpu/pool/mesh.hpp>
#include <renderer/gpu/pool/queue.hpp>

#include <renderer/gpu/pool/material.hpp>
#include <renderer/gpu/pool/shader.hpp>

#include <core/debug.hpp>

namespace
{
    Statistics g_statistics = {};
    bool g_initialized = false_value;
}

bool Gpu::init()
{
    if (g_initialized)
        return true_value;

    // Init
    DescriptorPool::init();
    MaterialPool::init();
    ShaderPool::init();
    RenderQueuePool::init();

    if (! Backend::init())
    {
        Debug::Println(
            PrintColorType_Red,
            "[Gpu] backend init falhou"
        );

        DescriptorPool::shutdown();
        return false_value;
    }

    g_statistics = {};
    g_initialized = true_value;

    Debug::Println(
        PrintColorType_Green,
        "[Gpu] inicializado"
    );

    return true_value;
}

void Gpu::shutdown()
{
    if (!g_initialized)
        return;

    Backend::shutdown();
    MaterialPool::shutdown();
    ShaderPool::shutdown();
    DescriptorPool::shutdown();

    g_statistics = {};
    g_initialized = false_value;
}

void Gpu::new_frame()
{
    g_statistics.reset_frame();
}

const Statistics& Gpu::statistics()
{
    return g_statistics;
}

DescriptorHandle Gpu::create_descriptor(const Descriptor& descriptor)
{
    return DescriptorPool::create(descriptor);
}

bool Gpu::update_descriptor(
    DescriptorHandle handle,
    const Descriptor& descriptor)
{
    return DescriptorPool::update(handle, descriptor);
}

bool Gpu::destroy_descriptor( DescriptorHandle handle )
{
    return DescriptorPool::destroy(handle);
}

const Descriptor* Gpu::get_descriptor( DescriptorHandle handle)
{
    return DescriptorPool::get(handle);
}

Descriptor* Gpu::get_descriptor_mutable( DescriptorHandle handle )
{
    return DescriptorPool::get(handle);
}

void Gpu::submit( CommandList& commad_list )
{
    if (!g_initialized)
    {
        Debug::Println(
            PrintColorType_Red,
            "[Gpu] submit antes de init"
        );
        return;
    }

    CommandDispatcher::execute(commad_list.items, commad_list.count);
    commad_list.reset();
}

void Gpu::execute(const Command& command)
{
    if (!g_initialized)
        return;

    CommandDispatcher::execute(command);
}

MaterialHandle Gpu::create_material( const Material& material )
{
    return MaterialPool::create( material );
}

bool Gpu::update_material( MaterialHandle handle, const Material& material )
{
    return MaterialPool::update( handle, material );
}

bool Gpu::destroy_material( MaterialHandle handle )
{
    return MaterialPool::destroy( handle );
}

Material* Gpu::get_material( MaterialHandle handle )
{
    return MaterialPool::get( handle );
}

const Material* Gpu::get_material_const( MaterialHandle handle )
{
    return MaterialPool::get_const( handle );
}

ShaderHandle Gpu::create_shader( const char* vs, const char* fs, const char* label )
{
    Shader s = {};
    if ( !Backend::shader_create_from_source( s, vs, fs ) )
        return {};
    if ( label )
        // copy label into s.label
    return ShaderPool::create( s );
}

bool Gpu::destroy_shader( ShaderHandle handle )
{
    Shader* s = ShaderPool::get( handle );
    if ( !s ) return false_value;
    Backend::shader_destroy( *s );
    return ShaderPool::destroy( handle );
}
