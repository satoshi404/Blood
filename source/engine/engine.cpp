#include <engine/engine.hpp>

#include <core/types.hpp>
#include <core/timer.hpp>
#include <core/debug.hpp>

#include <platform/window.hpp>
#include <platform/keyboard.hpp>

#include <renderer/factory.hpp>
#include <engine/system/node.hpp>
#include <engine/system/component.hpp>

#include <renderer/gpu/pool.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static RenderQueueHandle g_main_queue = {};
static Scene             g_main_scene = {};

namespace EngineBackend
{
    TimerScheduler scheduler;
    CommandList    frame_commands;
    uint_64        last_time, now, delta;
    constexpr uint_64 TARGET_FPS = 144ULL;
    constexpr uint_64 TARGET_FRAME_US = 1000000ULL / TARGET_FPS;
}

// Coleta recursiva: tudo que o runtime colocou na scene
static void collect_renderables( NodeHandle handle, RenderQueue* queue )
{
    Node* node = NodeSystem::get( handle );
    if ( !node || !node->active )
        return;

    if ( node->visible && node->descriptor.is_valid() )
    {
        if ( Descriptor* descriptor = Pool::get_descriptor( node->descriptor ) )
        {
            descriptor->transform = node->world;
            descriptor->dirty     = true_value;
        }
        queue->push( node->descriptor );
    }

    for ( uint_64 i = 0; i < node->children.size(); ++i )
        collect_renderables( node->children[ i ], queue );
}

bool Engine::init()
{
    if ( !CoreWindow::init() )  return false_value;
    if ( !Factory::init() )     return false_value;

    if ( !ComponentSystem::init() ) return false_value;
    if ( !g_main_scene.init( "MainScene" ) ) return false_value;

    g_main_queue = Factory::create_render_queue( "main" );
    if ( !g_main_queue.is_valid() )
        return false_value;

    CoreWindow::show();

    Factory::bind_command(
        Factory::make_viewport_command(
            0, 0,
            (int_32)WindowConfig::Get::width(),
            (int_32)WindowConfig::Get::height(),
            "main_viewport"
        )
    );

    EngineBackend::last_time = Timer::Get::microseconds();
    _start();   // runtime cria nodes / meshes aqui
    return true_value;
}

void Engine::update()
{
    while ( !CoreWindow::should_close() )
    {
        CoreWindow::pool();

        EngineBackend::now   = Timer::Get::microseconds();
        EngineBackend::delta = EngineBackend::now - EngineBackend::last_time;
        EngineBackend::last_time = EngineBackend::now;

        // 1) Input + lógica do jogo (runtime C++)
        if ( Keyboard::check( VK_Escape ) )
          CoreWindow::close();

        _input();
        _update( EngineBackend::delta );
        Keyboard::update( EngineBackend::delta );

        // 2) Scene graph
        g_main_scene.update_transforms();

        // 3) Coletar o que o runtime adicionou e renderizar
        RenderQueue* queue = Factory::get_render_queue( g_main_queue );
        if ( queue )
        {
            queue->clear();
            collect_renderables( g_main_scene.root, queue );
        }

        RenderPass pass = {};
        pass.viewport = {
            0, 0,
            (int_32)WindowConfig::Get::width(),
            (int_32)WindowConfig::Get::height()
        };
        pass.queue         = g_main_queue;
        pass.context_type  = ContextType_3D;
        pass.clear_enabled = true_value;
        pass.enabled       = true_value;
        pass.color.load_op  = LoadOpType_Clear;
        pass.color.store_op = StoreOpType_Store;
        pass.color.clear    = BlackColor;   // ou a cor que quiser de clear
        pass.depth.load_op  = LoadOpType_Clear;
        pass.depth.store_op = StoreOpType_DontCare;
        strncpy( pass.label, "MainPass", Limits::Label_Size - 1 );

        EngineBackend::frame_commands.reset();
        EngineBackend::frame_commands.push( Factory::make_begin_render_pass( pass, "begin" ) );
        EngineBackend::frame_commands.push( Factory::execute_queue( g_main_queue, "exec" ) );

        // Draw explícito de cada descriptor na queue (compatível com o teu dispatcher atual)
        if ( queue )
        {
            for ( uint_32 i = 0; i < queue->count; ++i )
                EngineBackend::frame_commands.push(
                    Factory::make_draw_command( queue->items[ i ], "draw" )
                );
        }

        EngineBackend::frame_commands.push( Factory::make_end_render_pass( "end" ) );
        EngineBackend::frame_commands.push( Factory::make_swap_command( "present" ) );
        Factory::submit( EngineBackend::frame_commands );

        EngineBackend::scheduler.update( EngineBackend::delta );
        _draw();

        // frame limit
        uint_64 frame_end = Timer::Get::microseconds();
        uint_64 elapsed   = frame_end - EngineBackend::now;
        if ( elapsed < EngineBackend::TARGET_FRAME_US )
            Timer::sleep_microseconds( EngineBackend::TARGET_FRAME_US - elapsed );
    }
}

void Engine::free()
{
    _finish();
    g_main_scene.free();
    ComponentSystem::shutdown();
    NodeSystem::shutdown();
    Factory::shutdown();
    CoreWindow::terminate();
}

Scene& Engine::scene() { return g_main_scene; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parse args
int main()
{
	if ( !Engine::init() ) return exit_failed_code;

	Engine::update();

	Engine::free();
	return exit_success_code;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////