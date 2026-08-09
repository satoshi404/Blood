#pragma once

#include <core/types.hpp>

#include <vendor/libc/stdarg.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum_type(PrintColor, u32){
	PrintColor_Default = 0,
	PrintColor_Black = 30,
	PrintColor_Red = 31,
	PrintColor_Green = 32,
	PrintColor_Yellow = 33,
	PrintColor_Blue = 34,
	PrintColor_Magenta = 35,
	PrintColor_Cyan = 36,
	PrintColor_White = 37,
};

#define color_format(color) ((u8)(color >> 24) << 16 | (u8)(color >> 16) << 8 | (u8)(color >> 8) & 0xFF)

namespace Debug
{
	extern void Print(const char *format, ...);
	extern void Print(PrintColor color, const char *format, ...);
	extern void Println(const char *format, ...);
	extern void Println(PrintColor color, const char *format, ...);
	extern void Print_formatted_variadic_color(bool newline, int color, const char *format, va_list args);
	extern void Print_formatted_variadic(bool newline, const char *format, va_list args);

	extern void Breakpoint();
	extern void Breakpoint(const char *message);

	extern void Log(const char *message);
	extern void Log(const char *message, const char *file, u32 line);
}

namespace Panic
{
	extern void Panic(const char *message, const char *file, u32 line);
	extern void Panic(const char *message, const char *file, u32 line, const char *function);
	extern void Panic(const char *message, const char *file, u32 line, const char *function, const char *expression);
	extern void Panic(const char *message, const char *file, u32 line, const char *function, const char *expression, const char *additional_info);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IfNullReturn(ptr, return_value, message)                \
	if (!(ptr))                                                 \
	{                                                           \
		Debug::Println(PrintColor_Red, "Error: %s\n", message); \
		return return_value;                                    \
	}

#define IfNullPanic(ptr, message)                                      \
	if (!(ptr))                                                        \
	{                                                                  \
		Panic::Panic(message, __FILE__, __LINE__, __FUNCTION__, #ptr); \
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////