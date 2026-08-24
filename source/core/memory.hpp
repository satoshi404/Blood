#pragma once

#include <core/types.hpp>

#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Memory
{
    INLINE void* alloc( unsigned_size size )
    {
        return malloc( size );
    }

    INLINE void* alloc_zero( unsigned_size size )
    {
        void* p = malloc( size );
        if ( p )
            memset( p, 0, size );
        return p;
    }

    INLINE void* realloc( void* ptr, unsigned_size size )
    {
        return ::realloc( ptr, size );
    }

    INLINE void free( void* ptr )
    {
        ::free( ptr );
    }

    INLINE void copy( void* dst, const void* src, unsigned_size size )
    {
        memcpy( dst, src, size );
    }

    INLINE void set( void* dst, int value, unsigned_size size )
    {
        memset( dst, value, size );
    }

    INLINE void zero( void* dst, unsigned_size size )
    {
        memset( dst, 0, size );
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////