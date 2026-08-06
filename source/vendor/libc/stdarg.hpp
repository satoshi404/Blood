#pragma once

#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// stdint.h

using va_list = __builtin_va_list;

#define VA_START( ap, last ) __builtin_va_start( ap, last )
#define VA_END( ap ) __builtin_va_end( ap )
#define VA_ARG( ap, type ) ( *(type*)__builtin_va_arg( ap, ( void* )sizeof( type ) ) )
#define VA_COPY( dest, src ) __builtin_va_copy( dest, src )

///////////////////////////////////////////////////////////////////////////////////////////////////////////
