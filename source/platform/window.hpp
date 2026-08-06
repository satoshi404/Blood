#pragma once

#include <core/types.hpp>

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

  extern void pool();
  extern void terminate();

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
    extern void size( const u32 width, const u32 height );
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
    extern i32 width();
    extern i32 height();
    extern const char* title();
    extern bool clipboard( char *buffer, usize size );
    extern bool selection( char *buffer, usize size );

    namespace Pixels
    {
      extern i32 width();
      extern i32 height();
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
