#pragma once

#include <core/types.hpp>

// TODO:
#include <vendor/libc/math.hpp>

INLINE const int iClamp( const int value, const int min, const int max )
{
	if ( value <= min ) return min;
	else if ( value >= max ) return max;
	return value;
}

INLINE const float fClamp( const float value, const float min, const float max )
{
	if ( value <= min ) return min;
	else if ( value >= max ) return max;
	return value;
}


template< typename T>
struct Mat_2Dim { T x; T y; };

template< typename T>
struct Mat_3Dim { T x; T y; T z; };

template< typename T>
struct Mat_4Dim { T x; T y; T z; T w; };
