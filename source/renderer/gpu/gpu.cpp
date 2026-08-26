#include <renderer/gpu/gpu.hpp>

#include <renderer/backend/backend.hpp>
#include <renderer/gpu/command/command.dispatch.hpp>

#include <renderer/gpu/pool/mesh.hpp>
#include <renderer/gpu/pool/queue.hpp>
#include <renderer/gpu/pool/shader.hpp>

#include <renderer/gpu/pool.hpp>

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

    ShaderPool::init();
    RenderQueuePool::init();

    Pool::init();

    if (! Backend::init())
    {
        Debug::Println(
            PrintColorType_Red,
            "[Gpu] backend init falhou"
        );

        ShaderPool::shutdown();
        RenderQueuePool::shutdown();

        Pool::free();

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
    ShaderPool::shutdown();
   // DescriptorPool::shutdown();

    Pool::free();

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
