#include <platform/window.hpp>
#include <core/debug.hpp>
#include <core/string.hpp>

#include <core/types.hpp>
// #include <core/memory.hpp>
#include <core/timer.hpp>
#include <platform/keyboard.hpp>

#include <renderer/gpu.factory.hpp>

// TODO: Windows error conflict strlen and strdup
// #include <vendor/libc/stdio.hpp>
// #include <vendor/libc/stdlib.hpp>
// #include <vendor/libc/string.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pipeline.hpp>

// TODO: Better implementation

enum_type(EngineMode, u32)
{
    EngineMode_Debug,
    EngineMode_Release,
};

#if 0
int main()
{
  String test;

  test.init( "blood" );
  test.append( "library", true_value ).append( "Ithink?" );

  const char* file = "blood.cpp";

  if ( !test.save_data( file ) )
  {
    Debug::Println( PrintColor_Red, "Failed save file %s", file );
    return exit_failed_code;
  }

  if ( !test.read_data( file ) ) {
    Debug::Println( PrintColor_Red, "Failed read file %s", file );
    return exit_failed_code; }

  Debug::Println( PrintColor_Cyan, test.data() );

  return exit_success_code;
}
#endif

#if 1

// TODO: Parse args
int main(int argc, char **argv)
{

  // TEST WINDOWS WINDOW
#if PLATFORM_WINDOWS
  CoreString testString = CoreString<const char *>("Hello, World");
  Debug::Println(PrintColor_Cyan, "Data: %s, Size: %llu", testString.data(), testString.size());

  if (!CoreWindow::init())
    return exit_failed_code;

  CoreWindow::show();

  while (!CoreWindow::should_close())
  {
    CoreWindow::pool();
  }

  CoreWindow::terminate();

#endif

  // WORK ON LINUX JUST
#if PLATFORM_LINUX
#if 1

  if (!CoreWindow::init())
    return exit_failed_code;

  CoreWindow::show();

#if RENDERER
  // Precisa vir depois de CoreWindow::init()/show(), pois e' so' ali que o
  // contexto GLX fica current (glXMakeCurrent) — antes disso glewInit()
  // (chamado dentro de GpuLayer::init()) falharia.
  if (!GpuFactory::init())
    return exit_failed_code;

  GpuCommand command_viewport = GpuFactory::make_viewport_command(
      0, 0,
      (i32)WindowConfig::Get::width(),
      (i32)WindowConfig::Get::height(),
      "main_viewport");

  GpuFactory::bind_command(command_viewport);

  // Materiais que o cubo pode usar — trocaveis a qualquer momento via
  // update_descriptor(), sem recriar nada.
  static GpuMaterial cube_material_normal = {{0.95f, 0.55f, 0.15f, 1.0f}, 0};
  static GpuMaterial cube_material_highlight = {{1.00f, 0.95f, 0.20f, 1.0f}, 0};

  static GpuTransform cube_transform = {
      .position = {0.0f, 0.0f, -5.0f},
      .rotation = {1, 1, 1},
      .scale = { 0.5, 0.5, 0.5 }
    };

  GpuDescriptor cube_descriptor = {};
  cube_descriptor.type = GpuDrawType_Cube;
  cube_descriptor.context = GpuContext_3D;
  cube_descriptor.material = &cube_material_normal; // mesh = nullptr -> usa o quad padrao do Cube
  cube_descriptor.transform = cube_transform;
  strncpy(cube_descriptor.label, "cube", sizeof(cube_descriptor.label) - 1);

  GpuDescriptorHandle cube_handle = GpuFactory::create_descriptor(cube_descriptor);

  GpuCommandList frame_commands;

#endif

  const EngineMode mode = EngineMode_Debug;

  static char base_title[256] = {};

  if (mode == EngineMode_Debug)
  {
    strncpy(base_title, "AxelEngine - Debug", sizeof(base_title) - 1);
    Debug::Println(PrintColor_Green, "[Mode] Debug");
  }
  else
  {
    strncpy(base_title, "AxelEngine - Release", sizeof(base_title) - 1);
    Debug::Println(PrintColor_Green, "[Mode] Release");
  }

  WindowConfig::Set::title(base_title);
  CoreWindow::update_config();
  Debug::Println(PrintColor_Cyan, "Here?");

  TimerScheduler main_scheduler;
  main_scheduler.schedule(1000000ULL, [](void *user_data)
                          {
        static u64 seconds_elapsed = 0;
        seconds_elapsed++;
        Debug::Println( PrintColor_Cyan, "[Uptime] %llu segundos", seconds_elapsed ); }, nullptr,
                          /*repeat=*/true);

  Debug::Println(PrintColor_Cyan, "Here2?");

  static bool title_changed = false_value;
  static u64 title_change_timer = 0;

  static char saved_base_title[256] = {};

  static char display_title[300] = {};
  static u64 fps_accum_us = 0;
  static u32 frame_count = 0;
  static u32 current_fps = 0;

  constexpr u64 TARGET_FPS = 144ULL;
  constexpr u64 TARGET_FRAME_US = 1000000ULL / TARGET_FPS;

  u64 last_time = Timer::Get::microseconds();

  #if RENDERER

  f64 angle = 0.;
  constexpr f64 ROTATION_SPEED = 4.0;

  #endif

  while (!CoreWindow::should_close())
  {

    u64 now = Timer::Get::microseconds();
    u64 delta = now - last_time;
    last_time = now;

    CoreWindow::pool();

#if RENDERER
    GpuFactory::new_frame();

    frame_commands.push(GpuFactory::make_clear_command(0.4f, 0.7f, 0.9f, 1.0f, "clear_bg"));
    frame_commands.push(GpuFactory::make_draw_command(cube_handle, "draw_cube"));
    frame_commands.push(GpuFactory::make_transform_command(1, 1, 1, "cube_trans"));
    frame_commands.push(GpuFactory::make_swap_command("present"));

    GpuFactory::submit(frame_commands);
#endif

    main_scheduler.update(delta);

    if (Keyboard::check_pressed(KeyCodeType_Space) && !title_changed)
    {
      strncpy(saved_base_title, base_title, sizeof(saved_base_title) - 1);
      strncpy(base_title, "YEAH", sizeof(base_title) - 1);

      title_changed = true_value;
      title_change_timer = 0;

#if RENDERER
      // Demonstra a troca do Descriptor em tempo real: o MESMO handle
      // passa a apontar pra outro material enquanto o titulo estiver em
      // "YEAH" — nenhum comando precisa ser recriado, o proximo
      // make_draw_command(cube_handle) ja' desenha com a cor nova.
      cube_descriptor.material = &cube_material_highlight;
      GpuFactory::update_descriptor(cube_handle, cube_descriptor);
#endif
    }

    if (Keyboard::check_pressed(KeyCodeType_Escape))
    {
      CoreWindow::close();
    }

    if (Keyboard::check_pressed(KeyCodeType_F))
    {

      bool is_fullscreen = WindowConfig::Get::is_fullscreen();
      if (!is_fullscreen)
        WindowConfig::Set::fullscreen(true_value);
      else
        WindowConfig::Set::fullscreen(false_value);

      CoreWindow::update_config();
      CoreWindow::change_resolution();
// Update viewport
#if RENDERER
      command_viewport = GpuFactory::make_viewport_command(
          0, 0,
          (i32)WindowConfig::Get::width(),
          (i32)WindowConfig::Get::height(),
          "main_viewport");
      GpuFactory::bind_command(command_viewport);
#endif
      Debug::Println(PrintColor_Green, "[Resolution:%s ] %ix%i", is_fullscreen ? "Windowed" : "Fullscreen",
                     WindowConfig::Get::width(),
                     WindowConfig::Get::height());
    }

    if (title_changed)
    {
      title_change_timer += delta;

      if (title_change_timer >= 3000000ULL)
      {
        strncpy(base_title, saved_base_title, sizeof(base_title) - 1);
        title_changed = false_value;

#if RENDERER
        cube_descriptor.material = &cube_material_normal;
        GpuFactory::update_descriptor(cube_handle, cube_descriptor);
#endif
      }
    }

    frame_count++;
    fps_accum_us += delta;

    if (fps_accum_us >= 1000000ULL)
    {
      current_fps = frame_count;
      frame_count = 0;
      fps_accum_us -= 1000000ULL;

      snprintf(display_title, sizeof(display_title), "%s | FPS: %u", base_title, current_fps);
      WindowConfig::Set::title(display_title);
      CoreWindow::update_config();
    }

    #if RENDERER

    angle += ROTATION_SPEED * (delta / 1'000'000.0);

    if (cube_descriptor.context == GpuContext_2D)
    {
        cube_transform.rotation[0] = static_cast<f32>(angle);
        cube_transform.rotation[1] = static_cast<f32>(angle);
        cube_transform.rotation[2] = 0;
    }
    else
    {
        cube_transform.rotation[0] = static_cast<f32>(angle);
        cube_transform.rotation[1] = static_cast<f32>(angle);
        cube_transform.rotation[2] = static_cast<f32>(angle);
    }

    //cube_transform.rotation[0] = static_cast<f32>(angle);
    //cube_transform.rotation[1] = static_cast<f32>(angle);
    //cube_transform.rotation[2] = static_cast<f32>(angle);
    cube_transform.mark_dirty();

    GpuTransformOps::update_matrix(cube_transform);

    cube_descriptor.transform = cube_transform;

    GpuFactory::update_descriptor(cube_handle, cube_descriptor);

    #endif

    Keyboard::update(delta);

    {
      u64 frame_end = Timer::Get::microseconds();
      u64 frame_elapsed = frame_end - now;

      if (frame_elapsed < TARGET_FRAME_US)
      {
        Timer::sleep_microseconds(TARGET_FRAME_US - frame_elapsed);
      }
    }
  }

#if RENDERER
  GpuFactory::destroy_descriptor(cube_handle);
  GpuFactory::shutdown();
#endif

  CoreWindow::terminate();
#endif
#endif

  return exit_success_code;
}

#endif