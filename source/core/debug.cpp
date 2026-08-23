#include "debug.hpp"

// TODO: Windows error conflict strlen and strdup
// #include <vendor/libc/stdio.hpp>
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Debug::Print_formatted_variadic_color(bool newline, int color, const char *format, va_list args)
{
	printf("\x1b[%dm", color);
	vprintf(format, args);
	printf(newline ? "\x1b[%dm\n" : "\x1b[%dm", PrintColorType_Default);
}

void Debug::Print_formatted_variadic(bool newline, const char *format, va_list args)
{
	vprintf(format, args);
	printf(newline ? "\n" : "");
}

void Debug::Print(const char *format, ...)
{
	va_list args;
	VA_START(args, format);
	Debug::Print_formatted_variadic(false_value, format, args);
	VA_END(args);
}

void Debug::Print(PrintColorType color_type, const char *format, ...)
{
	va_list args;
	VA_START( args, format );
	Debug::Print_formatted_variadic_color( false_value, color_type, format, args );
	VA_END(args);
}

void Debug::Println(const char *format, ...)
{
	va_list args;
	VA_START(args, format);
	Debug::Print_formatted_variadic(true_value, format, args);
	VA_END(args);
}

void Debug::Println(PrintColorType color_type, const char *format, ...)
{
	va_list args;
	VA_START(args, format);
	Debug::Print_formatted_variadic_color(true_value, color_type, format, args);
	VA_END(args);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
