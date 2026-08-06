#pragma once

#include <core/types.hpp>
#include <vendor/libc/stdarg.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// stdio.h

extern "C" void CDECL printf( const char* format, ... );
extern "C" void CDECL vprintf( const char* format, va_list args );
extern "C" void CDECL fprintf( void* stream, const char* format, ... );
extern "C" void CDECL vfprintf( void* stream, const char* format, va_list args );
extern "C" void CDECL sprintf( char* buffer, const char* format, ... );
extern "C" void CDECL vsprintf( char* buffer, const char* format, va_list args );
extern "C" void CDECL snprintf( char* buffer, usize size, const char* format, ... );
extern "C" void CDECL vsnprintf( char* buffer, usize size, const char* format, va_list args );

///////////////////////////////////////////////////////////////////////////////////////////////////////////