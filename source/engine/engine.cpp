#include <engine/engine.hpp>

#include <core/types.hpp>
#include <core/timer.hpp>
#include <core/debug.hpp>

#include <platform/window.hpp>
#include <platform/keyboard.hpp>

#include <renderer/gpu.factory.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ENGINE_COLOR_RENDERER  0.2f, 0.2f, 0.2f, 1.0f

// TODO: Test gpu renderer

static GpuMaterial simple_material = { { 0.95f, 0.55f, 0.15f, 1.0f }, 0 };

static GpuTransform simple_transform =
{
      .position = {0.0f, 0.0f, -5.0f},
      .rotation = {1, 1, 1},
      .scale = { 0.5, 0.5, 0.5 }
};

GpuDescriptorHandle simple_handle = {};
GpuDescriptor simple_descriptor = {};

namespace EngineBackend
{
	TimerScheduler scheduler;
	GpuCommandList frame_commands;

	u64 last_time;
	u64 now;
    u64 delta;
	f64 angle = 0.;

	// EngineRenderer 2D;

  	constexpr f64 ROTATION_SPEED = 4.0;
	constexpr u64 TARGET_FPS = 144ULL;
	constexpr u64 TARGET_FRAME_US = 1000000ULL / TARGET_FPS;
}

bool Engine::init()
{
	// Engine init
	if ( !CoreWindow::init() )
	{
		Debug::Println( PrintColor_Red, "Engine: Error init core window" );
		return exit_failed_code;
	}

	CoreWindow::show();

	if ( !GpuFactory::init() )
	{
		Debug::Println( PrintColor_Red, "Engine: Error init gpu renderer" );
		return exit_failed_code;
	}

	GpuCommand main_viewport = GpuFactory::make_viewport_command
	(
      	0, 0,
    	(i32)WindowConfig::Get::width(),
      	(i32)WindowConfig::Get::height(),
      	"main_viewport"
	);

  	GpuFactory::bind_command( main_viewport );

	EngineBackend::scheduler.schedule(1000000ULL, [](void *user_data) {
        static u64 seconds_elapsed = 0;
        seconds_elapsed++;
        Debug::Println( PrintColor_Cyan, "[Uptime] %llu segundos", seconds_elapsed ); }, nullptr,
                          /*repeat=*/true);

	EngineBackend::last_time = Timer::Get::microseconds();

	// Create
	auto factory_queue = GpuFactory::create_render_queue("Main");
	auto simple = GpuFactory::create_descriptor(
		GpuDescriptorBuilder()
			.mesh(  )
			.material( )
			.position( 100.0f, 200.f )
			.layer(1)
	);

	// Frame
	GpuRenderQueue* queue = GpuFactory::get_render_queue( factory_queue );
	queue->clear();
	queue->push( simple );

	GpuRenderPass pass = GpuFactory::make_simple_pass(
		0, 0, WindowConfig::Get::width(), WindowConfig::Get::height(),
		factory_queue,GpuContext_2D ,ENGINE_COLOR_RENDERER,
		"Pass"
	);



	// Runtime init
	_start();

	return exit_success_code;
}

void Engine::update()
{
	while (!CoreWindow::should_close())
  	{
		CoreWindow::pool();

		EngineBackend::now = Timer::Get::microseconds();
		EngineBackend::delta = EngineBackend::now - EngineBackend::last_time;
		EngineBackend::last_time = EngineBackend::now;

		EngineBackend::frame_commands.push( GpuFactory::make_clear_command( ENGINE_COLOR_RENDERER, "clear_bg" ) );
    	EngineBackend::frame_commands.push( GpuFactory::make_draw_command( simple_handle, "simple" ) );
    	EngineBackend::frame_commands.push( GpuFactory::make_transform_command( 1, 1, 1, "simple" ) );
    	EngineBackend::frame_commands.push( GpuFactory::make_swap_command( "present" ) );

		GpuFactory::submit( EngineBackend::frame_commands );

		EngineBackend::angle += 2.2 * (  EngineBackend::delta / 1'000'000.0);

		simple_transform.rotation[0] = static_cast<f32>( EngineBackend::angle );
        simple_transform.rotation[1] = static_cast<f32>( EngineBackend::angle );
        simple_transform.rotation[2] = 0;

		simple_transform.mark_dirty();

    	GpuTransformOps::update_matrix( simple_transform );

    	simple_descriptor.transform = simple_transform;

		GpuFactory::update_descriptor( simple_handle, simple_descriptor );

		EngineBackend::scheduler.update( EngineBackend::delta );

		// Engine input
		if ( Keyboard::check( VK_Escape ) )
		{
			Debug::Println( PrintColor_Cyan, "Stop engine" );
			EngineCall::stop();
		}

		// Runtime input
		_input();

		// Runtime update
		_update( EngineBackend::delta );

		// Engine update
		Keyboard::update( EngineBackend::delta );

		{
      		u64 frame_end = Timer::Get::microseconds();
      		u64 frame_elapsed = frame_end - EngineBackend::now;

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
	// Engine free
	GpuFactory::destroy_descriptor( simple_handle );
  	GpuFactory::shutdown();
	CoreWindow::terminate();

	// Runtime free
	_finish();
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
	// TODO:
	Engine::init();
	Engine::update();
	Engine::free();
	return exit_success_code;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////