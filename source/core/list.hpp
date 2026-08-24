#pragma once

#include <core/types.hpp>
#include <core/memory.hpp>

// Uses system malloc/realloc/free for now (same pattern as String)
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct List
{
    T*          data     = nullptr;
    uint_64     count    = 0;
    uint_64     capacity = 0;

    void init( uint_64 initial_capacity = 8 )
    {
        if ( initial_capacity < 1 )
            initial_capacity = 8;

        capacity = initial_capacity;
        count    = 0;
        data     = static_cast<T*>( malloc( capacity * sizeof( T ) ) );
    }

    void free()
    {
        if ( data )
        {
            ::free( data );
            data = nullptr;
        }
        count    = 0;
        capacity = 0;
    }

    void clear()
    {
        count = 0;
    }

    bool reserve( uint_64 new_capacity )
    {
        if ( new_capacity <= capacity )
            return true_value;

        T* new_data = static_cast<T*>( realloc( data, new_capacity * sizeof( T ) ) );
        if ( !new_data )
            return false_value;

        data     = new_data;
        capacity = new_capacity;
        return true_value;
    }

    bool ensure_capacity( uint_64 needed )
    {
        if ( needed <= capacity )
            return true_value;

        uint_64 new_cap = capacity ? capacity * 2 : 8;
        while ( new_cap < needed )
            new_cap *= 2;

        return reserve( new_cap );
    }

    // ------------------------------------------------------------------
    bool append( const T& value )
    {
        if ( !ensure_capacity( count + 1 ) )
            return false_value;

        data[ count++ ] = value;
        return true_value;
    }

    bool push( const T& value )
    {
        return append( value );
    }

    T pop()
    {
        if ( count == 0 )
            return T{};

        return data[ --count ];
    }

    // ------------------------------------------------------------------
    NO_DISCARD T& operator[]( uint_64 index )
    {
        return data[ index ];
    }

    NO_DISCARD const T& operator[]( uint_64 index ) const
    {
        return data[ index ];
    }

    NO_DISCARD T get( uint_64 index ) const
    {
        if ( index >= count )
            return T{};
        return data[ index ];
    }

    NO_DISCARD T* try_get( uint_64 index )
    {
        if ( index >= count )
            return nullptr;
        return &data[ index ];
    }

    NO_DISCARD uint_64 size() const
    {
        return count;
    }

    NO_DISCARD bool empty() const
    {
        return count == 0;
    }

    NO_DISCARD T* begin() { return data; }
    NO_DISCARD T* end()   { return data + count; }

    NO_DISCARD const T* begin() const { return data; }
    NO_DISCARD const T* end()   const { return data + count; }

    // ------------------------------------------------------------------
    // Fast remove (order not preserved)
    bool remove_at( uint_64 index )
    {
        if ( index >= count )
            return false_value;

        data[ index ] = data[ count - 1 ];
        --count;
        return true_value;
    }

    bool remove_swap( uint_64 index )
    {
        return remove_at( index );
    }

    // Ordered remove (preserves order, slower)
    bool remove_ordered( uint_64 index )
    {
        if ( index >= count )
            return false_value;

        for ( uint_64 i = index; i < count - 1; ++i )
            data[ i ] = data[ i + 1 ];

        --count;
        return true_value;
    }
};