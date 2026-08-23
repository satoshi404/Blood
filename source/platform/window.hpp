#pragma once

#include <core/types.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

#include <vendor/windows/windows.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace CoreWindow
{
  extern bool init();
  extern bool should_close();
  extern void change_resolution();
  extern void close();
  extern void show();

  extern void swap_buffers();

  extern void pool();
  extern void terminate();

  // TODO: Change it
  #if PLATFORM_LINUX
  extern void* get_gc();
  extern void* get_window();
  extern void* get_connection();
  #endif

  extern void update_config();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace WindowConfig
{
  namespace Show
  {
    extern void message( const char *title, const char *message );
    extern void message_error( const char *title, const char *message );
    extern void cursor( bool enabled );
  }

  namespace Set
  {
    extern void fullscreen( const bool value );
    extern void vsync( const bool value );
    extern void size( const uint_32 width, const uint_32 height );
    extern void title( const char* title );
    extern bool clipboard( const char *buffer );
    extern bool selection( const char *buffer );
    extern void caption( const char *caption );
    extern void scale( const float scale );
  }

  namespace Get
  {
    extern bool is_fullscreen();
    extern bool is_vsync();
    extern int_32 width();
    extern int_32 height();
    extern const char* title();
    extern bool clipboard( char *buffer, unsigned_size size );
    extern bool selection( char *buffer, unsigned_size size );

    namespace Pixels
    {
      extern int_32 width();
      extern int_32 height();
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
