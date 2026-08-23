#pragma once

#include <core/types.hpp>

#include <vendor/libc/stdarg.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum_type(PrintColorType, uint_32){
	PrintColorType_Default = 0,
	PrintColorType_Black = 30,
	PrintColorType_Red = 31,
	PrintColorType_Green = 32,
	PrintColorType_Yellow = 33,
	PrintColorType_Blue = 34,
	PrintColorType_Magenta = 35,
	PrintColorType_Cyan = 36,
	PrintColorType_White = 37,
};

#define color_format(color) ((u8)(color >> 24) << 16 | (u8)(color >> 16) << 8 | (u8)(color >> 8) & 0xFF)

namespace Debug
{
	extern void Print( const char *format, ... );
	extern void Print( PrintColorType color_type, const char *format, ... );
	extern void Println( const char *format, ... );
	extern void Println( PrintColorType color_type, const char *format, ...);
	extern void Print_formatted_variadic_color(bool newline, int color, const char *format, va_list args);
	extern void Print_formatted_variadic(bool newline, const char *format, va_list args);

	extern void Breakpoint();
	extern void Breakpoint( const char *message );

	extern void Log( const char *message );

	extern void Panic( const char *message );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IfNullReturn(ptr, return_value, message)                \
	if (!(ptr))                                                 \
	{                                                           \
		Debug::Println(PrintColorType_Red, "Error: %s\n", message); \
		return return_value;                                    \
	}\


#define IfNullPanic(ptr, message)                                      \
	if (!(ptr))                                                        \
	{                                                                  \
		Panic::Panic(message, __FILE__, __LINE__, __FUNCTION__, #ptr); \
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////