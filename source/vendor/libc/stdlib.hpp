#pragma once

#include <core/types.hpp>

// TODO: Better implementation

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// stdio.h

//!CONFLICT

#if 0
extern "C" void* CDECL malloc( usize size );
extern "C" void* CDECL calloc( usize count, usize size );
extern "C" void* CDECL realloc( void* ptr, usize size );
extern "C" void* CDECL aligned_alloc( usize alignment, usize size );
extern "C" void CDECL free( void* ptr );
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////