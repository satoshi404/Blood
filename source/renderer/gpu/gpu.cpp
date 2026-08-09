#include <renderer/gpu/gpu.hpp>

#include <renderer/layer/gpu.backend.hpp>
#include <renderer/gpu/pool/gpu.descriptor.pool.hpp>
#include <renderer/gpu/command/gpu.command.dispatch.hpp>
#include <core/debug.hpp>

namespace
{
    GpuStatistics g_statistics = {};
    bool g_initialized = false_value;
}

bool Gpu::init()
{
    if (g_initialized)
        return true_value;

    GpuDescriptorPool::init();

    if (!GpuBackend::init())
    {
        Debug::Println(
            PrintColor_Red,
            "[Gpu] backend init falhou"
        );

        GpuDescriptorPool::shutdown();
        return false_value;
    }

    g_statistics = {};
    g_initialized = true_value;

    Debug::Println(
        PrintColor_Green,
        "[Gpu] inicializado"
    );

    return true_value;
}

void Gpu::shutdown()
{
    if (!g_initialized)
        return;

    GpuBackend::shutdown();
    GpuDescriptorPool::shutdown();

    g_statistics = {};
    g_initialized = false_value;
}

void Gpu::new_frame()
{
    g_statistics.reset_frame();
}

const GpuStatistics& Gpu::statistics()
{
    return g_statistics;
}

GpuDescriptorHandle Gpu::create_descriptor(const GpuDescriptor& descriptor)
{
    return GpuDescriptorPool::create(descriptor);
}

bool Gpu::update_descriptor(
    GpuDescriptorHandle handle,
    const GpuDescriptor& descriptor)
{
    return GpuDescriptorPool::update(handle, descriptor);
}

bool Gpu::destroy_descriptor(GpuDescriptorHandle handle)
{
    return GpuDescriptorPool::destroy(handle);
}

const GpuDescriptor* Gpu::get_descriptor(GpuDescriptorHandle handle)
{
    return GpuDescriptorPool::get(handle);
}

GpuDescriptor* Gpu::get_descriptor_mutable(GpuDescriptorHandle handle)
{
    return GpuDescriptorPool::get(handle);
}

void Gpu::submit(GpuCommandList& list)
{
    if (!g_initialized)
    {
        Debug::Println(
            PrintColor_Red,
            "[Gpu] submit antes de init"
        );
        return;
    }

    GpuCommandDispatcher::execute(list.items, list.count);
    list.reset();
}

void Gpu::execute(const GpuCommand& command)
{
    if (!g_initialized)
        return;

    GpuCommandDispatcher::execute(command);
}
