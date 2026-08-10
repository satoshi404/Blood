#pragma once

#include <core/types.hpp>

// TODO:
#include <vendor/libc/math.hpp>

template< typename T>
struct Mat_2Dim { T x; T y; };

template< typename T>
struct Mat_3Dim { T x; T y; T z; };

template< typename T>
struct Mat_4Dim { T x; T y; T z; T w; };
