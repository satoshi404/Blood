#include <platform/window.hpp>
#include <platform/xcb/glx.hpp>
#include <platform/keyboard.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

#include <core/debug.hpp>

#if PLATFORM_LINUX

#if RENDERER

#include <GL/glx.h>

#endif

#include <vendor/xcb/xcb.hpp>
#include <vendor/xcb/xproto.hpp>
#include <vendor/xcb/keysyms.hpp>
#include <vendor/x11/xutil.hpp>

// TODO: Windows error conflict strlen and strdup
// #include <vendor/libc/stdio.hpp>
// #include <vendor/libc/stdlib.hpp>
// #include <vendor/libc/string.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: vendor
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

struct WindowHandler
{
  XCB_Connection *connection;
  XCB_Window window;

  Display *display;
  XCB_Window root;

  struct
  {
    XCB_Atom wm_delete_window;
    XCB_Atom protocols;
    XCB_Atom wm_state;
    XCB_Atom wm_state_fullscreen;
    // XCB_Atom wm_icon;
  } atoms;

  XCB_KeySymbols *key_symbols;
  XCB_GenericEvent *event;
  XCB_Screen *screen;

  bool is_running;
};

GLOBAL WindowHandler window_handle;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static XCB_Atom intern_atom(XCB_Connection *connection, const char *atom_name)
{
  bool only_if_exists = false_value;

  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, only_if_exists, strlen(atom_name), atom_name);
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, NULL);

  if (!reply)
  {
    Debug::Println(PrintColor_Red, "Error: Failed to intern atom '%s'", atom_name);
    return static_cast<XCB_Atom>(XCB_ATOM_NONE);
  }

  XCB_Atom atom = static_cast<XCB_Atom>(reply->atom);
  free(reply);

  return atom;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CoreWindow::init()
{
  window_handle = {0};

  IfNullReturn(window_handle.display = XOpenDisplay(nullptr), false_value, "Failed to open X display");

  if (!(window_handle.connection = XGetXCBConnection(window_handle.display)))
  {

    Debug::Println(PrintColor_Red, "Error: Connect to xcb server");

    XCloseDisplay(window_handle.display);
    window_handle.display = nullptr;

    return false_value;
  }

// TODO: Better implementation
#if RENDERER && API_OPENGL

  int visual_attribs[] =
      {
          GLX_RGBA,
          GLX_DOUBLEBUFFER,
          GLX_DEPTH_SIZE, 24,
          GLX_STENCIL_SIZE, 8,
          None};

  XVisualInfo *visual = glXChooseVisual(
      window_handle.display,
      DefaultScreen(window_handle.display),
      visual_attribs);

  IfNullReturn(visual, false_value, "GlXChooseVisual failed");

#endif

  const XCB_Setup *setup = xcb_get_setup(window_handle.connection);
  XCB_ScreenIterator iter = xcb_setup_roots_iterator(setup);
  window_handle.screen = iter.data;

  window_handle.window = xcb_generate_id(window_handle.connection);
  window_handle.root = window_handle.screen->root;

// TODO: Better implementation
#if RENDERER && API_OPENGL

  xcb_colormap_t colormap = xcb_generate_id(window_handle.connection);
  xcb_create_colormap(
      window_handle.connection,
      XCB_COLORMAP_ALLOC_NONE,
      colormap,
      window_handle.screen->root,
      visual->visualid);

  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
  uint32_t value_list[3];
  value_list[0] = window_handle.screen->white_pixel;
  value_list[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;
  value_list[2] = colormap;

  xcb_create_window(
      window_handle.connection,
      visual->depth,
      window_handle.window,
      window_handle.screen->root,
      0, 0,
      WindowConfig::Get::width(), WindowConfig::Get::height(),
      0,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      visual->visualid,
      value_mask,
      value_list);

  GLXDrawable drawable = (GLXDrawable)window_handle.window;

  GLXContext context = glXCreateContext(
      window_handle.display,
      visual,
      nullptr,
      true_value);

  IfNullReturn(context, false_value, "Error: GlXChooseContext failed");

  glXMakeCurrent(
      window_handle.display,
      drawable,
      context);

#else

  uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t value_list[2];
  value_list[0] = window_handle.screen->white_pixel; // White background
  value_list[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE;

  xcb_create_window(
      window_handle.connection,
      XCB_COPY_FROM_PARENT,
      window_handle.window,
      window_handle.screen->root,
      0, 0,
      WindowConfig::Get::width(), WindowConfig::Get::height(),
      0,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      window_handle.screen->root_visual,
      value_mask, value_list);

#endif

  X11_SetFixedSize_Hints(window_handle.connection, window_handle.window, WindowConfig::Get::width(), WindowConfig::Get::height());

  {
    const char instance_class[] = "Axel\0Engine";
    xcb_change_property(
        window_handle.connection,
        XCB_PROP_MODE_REPLACE,
        window_handle.window,
        XCB_ATOM_WM_CLASS,
        XCB_ATOM_STRING,
        8,
        sizeof(instance_class),
        instance_class);
  }

  // Atom Delete Window
  {
    window_handle.atoms.wm_delete_window = intern_atom(window_handle.connection, "WM_DELETE_WINDOW");
  }

  // Atom State Fullscreen
  {
    window_handle.atoms.wm_state_fullscreen = intern_atom(window_handle.connection, "_NET_WM_STATE_FULLSCREEN");
  }

  // Atom Protocols
  {
    window_handle.atoms.protocols = intern_atom(window_handle.connection, "WM_PROTOCOLS");
  }

  // Atom State
  {
    window_handle.atoms.wm_state = intern_atom(window_handle.connection, "_NET_WM_STATE");
  }

  // Set atoms for the window
  {
    xcb_change_property(
        window_handle.connection,
        XCB_PROP_MODE_REPLACE,
        window_handle.window,
        window_handle.atoms.protocols,
        XCB_ATOM_ATOM,
        32,
        1,
        &window_handle.atoms.wm_delete_window);

    xcb_flush(window_handle.connection);
  }

  // Update
  CoreWindow::update_config();

  window_handle.key_symbols = xcb_key_symbols_alloc(window_handle.connection);
  window_handle.is_running = true_value;

  Debug::Println(PrintColor_Yellow, "Window XCB:");
  Debug::Println(PrintColor_Yellow, "XCB Connection %p", window_handle.connection);
  Debug::Println(PrintColor_Yellow, "XCB WINDOW %p", window_handle.window);

  return true_value;
}

void CoreWindow::show()
{
  // Show
  xcb_map_window(window_handle.connection, window_handle.window);
  xcb_flush(window_handle.connection);
}

void CoreWindow::swap_buffers()
{
#if RENDERER && API_OPENGL
  glXSwapBuffers(window_handle.display, (GLXDrawable)window_handle.window);

#elif !RENDERER
  xcb_flush(window_handle.connection);

#endif
}

void CoreWindow::update_config()
{
  Debug::Println(PrintColor_Green, "%ix%i", WindowConfig::Get::width(), WindowConfig::Get::height());

  // Size config window update
  {
    i32 dims[] = {WindowConfig::Get::width(), WindowConfig::Get::height()};
    xcb_configure_window(window_handle.connection, window_handle.window,
                         XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, dims);
    xcb_flush(window_handle.connection);
  }

  // Title config window update
  {
    const char *title = WindowConfig::Get::title();
    xcb_change_property(
        window_handle.connection,
        XCB_PROP_MODE_REPLACE,
        window_handle.window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        strlen(title),
        title);

    xcb_flush(window_handle.connection);
  }

  // Fullscreen config window update
  {
    xcb_client_message_event_t event = {};
    event.response_type = XCB_CLIENT_MESSAGE;
    event.format = 32;
    event.window = window_handle.window;
    event.type = window_handle.atoms.wm_state;
    event.data.data32[0] = WindowConfig::Get::is_fullscreen() ? 1 : 0; // 1 = _NET_WM_STATE_ADD, 0 = _NET_WM_STATE_REMOVE
    event.data.data32[1] = window_handle.atoms.wm_state_fullscreen;
    event.data.data32[2] = 0;
    event.data.data32[3] = 1;
    event.data.data32[4] = 0;

    xcb_send_event(
        window_handle.connection,
        false_value,
        window_handle.root,
        XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
        (const char *)&event);

    xcb_flush(window_handle.connection);
  }
}

void CoreWindow::change_resolution()
{
  if (WindowConfig::Get::is_fullscreen())
    WindowConfig::Set::size(window_handle.screen->width_in_pixels,
                            window_handle.screen->height_in_pixels);
  else
    WindowConfig::Set::size(WINDOW_DEFAULT_WIDTH,
                            WINDOW_DEFAULT_HEIGHT);

  CoreWindow::update_config();
}

bool CoreWindow::should_close()
{
  return !window_handle.is_running;
}

void CoreWindow::close()
{
  window_handle.is_running = false_value;
}

void CoreWindow::pool()
{
  while ((window_handle.event = xcb_poll_for_event(window_handle.connection)))
  {
    switch (window_handle.event->response_type & ~0x80)
    {
    case XCB_CLIENT_MESSAGE:
    {
      xcb_client_message_event_t *client_message = (xcb_client_message_event_t *)window_handle.event;

      if (client_message->data.data32[0] == window_handle.atoms.wm_delete_window)
      {
        window_handle.is_running = false_value;
      }
    }
    break;

    case XCB_KEY_PRESS:
    {
      xcb_key_press_event_t *kp = (xcb_key_press_event_t *)window_handle.event;
      xcb_keysym_t keysym = xcb_key_press_lookup_keysym(window_handle.key_symbols, kp, 0);
      u8 key = keysym & 0xFF;

      Keyboard::state().keyCurrent[key] = true_value;
    }
    break;

    case XCB_KEY_RELEASE:
    {
      xcb_key_release_event_t *kr = (xcb_key_release_event_t *)window_handle.event;
      xcb_keysym_t keysym = xcb_key_press_lookup_keysym(window_handle.key_symbols, kr, 0);
      u8 key = keysym & 0xFF;

      Keyboard::state().keyCurrent[key] = false_value;
    }
    break;

    default:
      break;
    }

    free(window_handle.event);
  }
}

void CoreWindow::terminate()
{
  if (window_handle.key_symbols)
  {
    xcb_key_symbols_free(window_handle.key_symbols);
    window_handle.key_symbols = nullptr;
  }

  if (window_handle.window)
  {
    xcb_destroy_window(window_handle.connection, window_handle.window);
    window_handle.window = 0;
  }

  if (window_handle.display)
  {
    XCloseDisplay(window_handle.display);
    window_handle.display = nullptr;
  }

  window_handle.connection = nullptr;
  window_handle.is_running = false_value;
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
