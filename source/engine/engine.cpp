#include <engine/engine.hpp>

#include <core/types.hpp>
#include <core/timer.hpp>
#include <core/debug.hpp>

#include <platform/window.hpp>
#include <platform/keyboard.hpp>

#include <renderer/factory.hpp>
#include <engine/system/node.hpp>
#include <engine/system/component.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ENGINE_COLOR_RENDERER  0.2f, 0.2f, 0.2f, 1.0f

// TODO: Test gpu renderer

static DescriptorHandle simple_handle = {};
static RenderQueueHandle simple_queue = {};
static Transform simple_transform = {};

static Scene g_main_scene = {};

namespace EngineBackend
{
	TimerScheduler scheduler;
	CommandList frame_commands;

	uint_64 last_time;
	uint_64 now;
    uint_64 delta;
	float_64 angle = 0.;

  	constexpr float_64 ROTATION_SPEED = 4.0;
	constexpr uint_64 TARGET_FPS = 144ULL;
	constexpr uint_64 TARGET_FRAME_US = 1000000ULL / TARGET_FPS;
}

bool Engine::init()
{
	// Window
	if ( !CoreWindow::init() )
	{
		Debug::Println( PrintColorType_Red, "Engine: Error init core window" );
		return false_value;
	}

	// GPU
	if ( !Factory::init() )
	{
		Debug::Println( PrintColorType_Red, "Engine: Error init gpu renderer" );
		return false_value;
	}

	// Node System + Component System + Scene
	if ( !ComponentSystem::init() )
	{
		Debug::Println( PrintColorType_Red, "Engine: Error init ComponentSystem" );
		return false_value;
	}

	if ( !g_main_scene.init( "MainScene" ) )
	{
		Debug::Println( PrintColorType_Red, "Engine: Error init NodeSystem / Scene" );
		return false_value;
	}

	CoreWindow::show();

	// Temporary test objects (will be replaced by proper scene nodes later)
	simple_queue = Factory::create_render_queue( "queue" );
	if ( !simple_queue.is_valid() )
	{
		Engine::free();
		Debug::Println( PrintColorType_Red, "Engine: FAILED CREATE RenderQueue" );
		return false_value;
	}

	simple_transform = {};
	simple_transform.position = fvec3( 0.f, 0.f, -4.f );
	simple_transform.scale = fvec3( .8f, .8f, .8f );
	simple_transform.mark_dirty();

	simple_handle = Factory::create_descriptor(
		DescriptorBuilder()
			.type( DrawType_Cube )
			.context( ContextType_3D )
			.label("cube")
			.build()
	);

	Factory::bind_command(
		Factory::make_viewport_command(
			0, 0,
			(int_32)WindowConfig::Get::width(),
			(int_32)WindowConfig::Get::height(),
			"main_viewport"
		)
	);

	EngineBackend::last_time = Timer::Get::microseconds();

	EngineBackend::scheduler.schedule(1000000ULL, [](void *user_data) {
        static uint_64 seconds_elapsed = 0;
        seconds_elapsed++;
        Debug::Println( PrintColorType_Cyan, "[Uptime] %llu segundos", seconds_elapsed ); }, nullptr,
                          /*repeat=*/true);

	// Runtime init
	_start();

	Debug::Println( PrintColorType_Green, "Engine: initialized (nodes: %llu, components: %llu)",
		NodeSystem::count(), ComponentSystem::count() );
	return true_value;
}

void Engine::update()
{
	while (!CoreWindow::should_close())
  	{
		CoreWindow::pool();

		EngineBackend::now = Timer::Get::microseconds();
		EngineBackend::delta = EngineBackend::now - EngineBackend::last_time;
		EngineBackend::last_time = EngineBackend::now;

		EngineBackend::angle += 2.2 * (  EngineBackend::delta / 1'000'000.0);

		simple_transform.rotation = fvec3(
			 (float_32)( EngineBackend::angle ),
             (float_32)( EngineBackend::angle * .6f ),
             0.f
		);
		simple_transform.mark_dirty();

		if ( Descriptor* des = Factory::get_descriptor_mutable( simple_handle ) )
		{
			des->transform = simple_transform;
			des->dirty = true_value;
		}

		RenderQueue* queue = Factory::get_render_queue( simple_queue );
		if ( queue )
		{
			queue->clear();
			queue->push( simple_handle );
		}


		RenderPass pass = {};
		pass.viewport =
		{
			0, 0,
			(int_32)WindowConfig::Get::width(),
			(int_32)WindowConfig::Get::height()
		};
		pass.queue = simple_queue;
		pass.context_type = ContextType_3D;
		pass.clear_enabled = true_value;
		pass.enabled = true_value;

		pass.color.load_op = LoadOpType_Clear;
		pass.color.store_op = StoreOpType_Store;
		pass.color.clear = GreenColor;

		pass.depth.load_op = LoadOpType_Clear;
		pass.depth.store_op = StoreOpType_DontCare;

		strncpy( pass.label, "MainPass", Limits::Label_Size - 1 );

		EngineBackend::frame_commands.reset();

		EngineBackend::frame_commands.push(
			Factory::make_begin_render_pass( pass, "begin_main" )
		);
    	EngineBackend::frame_commands.push( Factory::execute_queue( simple_queue, "exec_main_queue" ) );
		EngineBackend::frame_commands.push( Factory::make_draw_command( simple_handle, "cube" ) );
    	EngineBackend::frame_commands.push( Factory::make_end_render_pass( "end_main" ) );
    	EngineBackend::frame_commands.push( Factory::make_swap_command( "present" ) );

		Factory::submit( EngineBackend::frame_commands );

		EngineBackend::scheduler.update( EngineBackend::delta );

		// Engine input
		if ( Keyboard::check( VK_Escape ) )
		{
			Debug::Println( PrintColorType_Cyan, "Stop engine" );
			EngineCall::stop();
		}

		// Runtime input
		_input();

		// Runtime update
		_update( EngineBackend::delta );

		// Scene / Node transforms
		g_main_scene.update_transforms();

		// Components (scripts etc.)
		ComponentSystem::update_all( EngineBackend::delta );

		// Engine update
		Keyboard::update( EngineBackend::delta );

		{
      		uint_64 frame_end = Timer::Get::microseconds();
      		uint_64 frame_elapsed = frame_end - EngineBackend::now;

      		if ( frame_elapsed < EngineBackend::TARGET_FRAME_US )
      		{
      		  Timer::sleep_microseconds( EngineBackend::TARGET_FRAME_US - frame_elapsed );
      		}
    	}

		// Runtime draw
		_draw();
	}
}

void Engine::free()
{
	// Runtime free first
	_finish();

	// Scene + systems
	g_main_scene.free();
	ComponentSystem::shutdown();
	NodeSystem::shutdown();

	// GPU + Window
	Factory::shutdown();
	CoreWindow::terminate();
}

Scene& Engine::scene()
{
	return g_main_scene;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void EngineCall::stop()
{
	CoreWindow::close();
}

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