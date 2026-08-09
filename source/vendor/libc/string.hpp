#pragma once

#include <core/types.hpp>

// TODO: Better implementation

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// string.h

// !CONFLICT

#if 0
extern "C" void CDECL *memset( void *str, int c, usize size );
//extern "C" isize CDECL  strlen( const char* c );
extern "C" void* CDECL memcpy( void *dest, const void *src, usize size );
//extern "C" const char* CDECL strdup( const char* c );
extern "C" int CDECL strcmp( const char* str1, const char* str2 );
extern "C" int CDECL strncmp( const char* str1, const char* str2, usize n );
extern "C" char* CDECL strcpy( char *dest, const char *src );
extern "C" char* CDECL strncpy( char *dest, const char *src, usize n );
extern "C" char* CDECL strcat( char *dest, const char *src );
extern "C" char* CDECL strncat( char *dest, const char *src, usize n );
extern "C" void * CDECL memmove( void *dest, const void *src, usize n );
extern "C" int CDECL memcmp( const void *ptr1, const void *ptr2, usize n );
#endif

//extern "C" char* CDECL strchr( const char *str, int c );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
