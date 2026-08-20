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


static GpuDescriptorHandle simple_handle = {};
static GpuRenderQueueHandle simple_queue = {};
static GpuTransform simple_transform = {};

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
		return false_value;
	}

	if ( !GpuFactory::init() )
	{
		Debug::Println( PrintColor_Red, "Engine: Error init gpu renderer" );
		return false_value;
	}

	CoreWindow::show();

	// Create
	simple_queue = GpuFactory::create_render_queue( "queue" );
	if ( !simple_handle.is_valid() )
	{
		Engine::free();
		Debug::Println( PrintColor_Red, "Engine: FAILED CREATE RenderQueue" );
		return false_value;
	}

	simple_transform = {};
	simple_transform.position[0] = 0.;
	simple_transform.position[1] = 0.;
	simple_transform.position[2] = -5.;
	simple_transform.scale[0] = 0.08;
	simple_transform.scale[1] = 0.08;
	simple_transform.scale[2] = 0.08;
	simple_transform.mark_dirty();


	simple_handle = GpuFactory::create_descriptor(
		GpuDescriptorBuilder()
			.type( GpuDrawType_Cube )
			.context( GpuContext_3D )
			.position( 100.0f, 200.f )
			.material( {} )
			.scale( 0.08, 0.08, 0.08 )
			.label("cube")
			.build()
	);

	GpuFactory::bind_command(
		GpuFactory::make_viewport_command(
			0, 0,
			(i32)WindowConfig::Get::width(),
			(i32)WindowConfig::Get::height(),
			"main_viewport"
		)
	);

	EngineBackend::last_time = Timer::Get::microseconds();


	EngineBackend::scheduler.schedule(1000000ULL, [](void *user_data) {
        static u64 seconds_elapsed = 0;
        seconds_elapsed++;
        Debug::Println( PrintColor_Cyan, "[Uptime] %llu segundos", seconds_elapsed ); }, nullptr,
                          /*repeat=*/true);

	// Runtime init
	_start();

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

		simple_transform.rotation[0] = (f32)( EngineBackend::angle );
        simple_transform.rotation[1] = (f32)( EngineBackend::angle * 0.6 );
        simple_transform.rotation[2] = 0;
		simple_transform.mark_dirty();

		if ( GpuDescriptor* des = GpuFactory::get_descriptor_mutable( simple_handle ) )
		{
			des->transform = simple_transform;
			des->dirty = true_value;
		}

		GpuRenderQueue* queue = GpuFactory::get_render_queue( simple_queue );
		if ( queue )
		{
			queue->clear();
			queue->push( simple_handle );
		}


		GpuRenderPass pass = {};
		pass.viewport =
		{
			0, 0,
			(i32)WindowConfig::Get::width(),
			(i32)WindowConfig::Get::height()
		};
		pass.queue = simple_queue;
		pass.context = GpuContext_3D;
		pass.clear_enabled = true_value;
		pass.enabled = true_value;

		pass.color.load_op = GpuLoadOp_Clear;
		pass.color.store_op = GpuStoreOp_Store;
		pass.color.clear = { 0.15, 0.15, 1.0, 1.0 };

		pass.depth.load_op = GpuLoadOp_Clear;
		pass.depth.store_op = GpuStoreOp_DontCare;

		strncpy( pass.label, "MainPass", GpuLimits::LabelSize - 1 );

		EngineBackend::frame_commands.reset();

		EngineBackend::frame_commands.push(
			GpuFactory::make_begin_render_pass( pass, "begin_main" )
		);
    	EngineBackend::frame_commands.push( GpuFactory::execute_queue( simple_queue, "exec_main_queue" ) );
    	EngineBackend::frame_commands.push( GpuFactory::make_end_render_pass( "end_main" ) );
    	EngineBackend::frame_commands.push( GpuFactory::make_swap_command( "present" ) );

		GpuFactory::submit( EngineBackend::frame_commands );

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
	//GpuFactory::destroy_descriptor( simple_handle );
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
	if ( !Engine::init() ) return exit_failed_code;

	Engine::update();

	Engine::free();
	return exit_success_code;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////