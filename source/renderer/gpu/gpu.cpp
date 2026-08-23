#include <renderer/gpu/gpu.hpp>

#include <renderer/layer/backend.hpp>
#include <renderer/gpu/pool/descriptor.hpp>
#include <renderer/gpu/command/command.dispatch.hpp>
#include <renderer/gpu/pool/mesh.hpp>
#include <renderer/gpu/pool/queue.hpp>
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
