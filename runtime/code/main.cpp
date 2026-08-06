#include <platform/window.hpp>
#include <core/debug.hpp>
#include <core/string.hpp>

#include <core/types.hpp>
//#include <core/memory.hpp>
#include <core/timer.hpp>
#include <platform/keyboard.hpp>

#include <renderer/gpu.hpp>

// TODO: Windows error conflict strlen and strdup
//#include <vendor/libc/stdio.hpp>
//#include <vendor/libc/stdlib.hpp>
//#include <vendor/libc/string.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <pipeline.hpp>

// TODO: Better implementation



enum_type(EngineMode, u32){
    EngineMode_Debug,
    EngineMode_Release,
};

// TODO: Parse args

int main(int argc, char **argv)
{
  CoreString testString = CoreString<const char *>("Hello, World");
  Debug::Println(PrintColor_Cyan, "Data: %s, Size: %llu", testString.data(), testString.size());


  if (!CoreWindow::init())
  return exit_failed_code;

  CoreWindow::show();

  while (!CoreWindow::should_close())
  {
    CoreWindow::pool();
  }

#if 0

#if 0
  GpuLayer_Draw draw =
  {
    .context = DrawContext_2D,
    .type = DrawCommandType_Cube
  };

  GpuLayer_Command command
  {
    .type = CommandType_Draw,
    .draw = draw
  };

  GpuLayer::bind_command( command );

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

  TimerScheduler main_scheduler;
  main_scheduler.schedule(1000000ULL, [](void *user_data)
  {
        static u64 seconds_elapsed = 0;
        seconds_elapsed++;
        Debug::Println( PrintColor_Cyan, "[Uptime] %llu segundos", seconds_elapsed );
  }, nullptr,
  /*repeat=*/true
  );

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

  while (!CoreWindow::should_close())
  {

    u64 now = Timer::Get::microseconds();
    u64 delta = now - last_time;
    last_time = now;

    CoreWindow::pool();

    main_scheduler.update(delta);

    // TODO: Work on linux yet
    #if PLATFORM_LINUX
    if (Keyboard::check_pressed(KeyCodeType_Space) && !title_changed)
    {
      strncpy(saved_base_title, base_title, sizeof(saved_base_title) - 1);
      strncpy(base_title, "YEAH", sizeof(base_title) - 1);

      title_changed = true_value;
      title_change_timer = 0;
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
      }
    }

    #endif

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


#endif

  CoreWindow::terminate();

  return exit_success_code;
}