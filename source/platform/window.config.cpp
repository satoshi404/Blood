#include <platform/window.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Window configuration data

static struct WindowConfigData
{
	bool fullscreen;
	bool vsync;
	u32 width;
	u32 height;
	const char* title;
	float scale;

} window_config_data =
{
	false_value,  // fullscreen
	false_value,  // vsync
	WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, WINDOW_DEFAULT_TITLE,  // width, height, title
	1.0f, // scale
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set

void WindowConfig::Set::fullscreen( const bool value )
{
	window_config_data.fullscreen = value;
}

void WindowConfig::Set::vsync( const bool value )
{
	window_config_data.vsync = value;
}

void WindowConfig::Set::size( const u32 width, const u32 height )
{
	window_config_data.width = width;
	window_config_data.height = height;
}

void WindowConfig::Set::scale( const float scale )
{
	window_config_data.scale = scale;
}

void WindowConfig::Set::title( const char* title )
{
	window_config_data.title = title;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get

bool WindowConfig::Get::is_fullscreen()
{
	return window_config_data.fullscreen;
}

bool WindowConfig::Get::is_vsync()
{
	return window_config_data.vsync;
}

i32 WindowConfig::Get::width()
{
	return window_config_data.width;
}

i32 WindowConfig::Get::height()
{
	return window_config_data.height;
}

i32 WindowConfig::Get::Pixels::width()
{
	return static_cast<i32>( window_config_data.width * window_config_data.scale );
}

i32 WindowConfig::Get::Pixels::height()
{
	return static_cast<i32>( window_config_data.height * window_config_data.scale );
}

const char* WindowConfig::Get::title()
{
	return window_config_data.title;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
