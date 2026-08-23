#pragma once

#include <pipeline.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline and no discard macros

#define INLINE inline
#define ALWAYS_INLINE __attribute__((always_inline)) inline

#define NO_DISCARD [[ nodiscard ]]
#define NO_INLINE __attribute__((noinline))
#define NO_RETURN __attribute__((noreturn))
#define UNUSED __attribute__((unused))
#define PACKED __attribute__((packed))

#define ALIGN( alignment ) __attribute__((aligned(alignment)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Signed Integer types

using int_8 = signed char;
#define INT8_MAX ( 127 )              // 0x7F
#define INT8_MIN ( -128 )             // 0x80

using int_16 = signed short;
#define INT16_MAX ( 32767 )           // 0x7FFF
#define INT16_MIN ( -32768 )          // 0x8000

using int_32 = signed int;
#define INT32_MAX ( 2147483647 )      // 0x7FFFFFFF
#define INT32_MIN ( -2147483647 - 1 )

using int_64 = signed long long;
#define INT64_MAX ( 9223372036854775807LL )  // 0x7FFFFFFFFFFFFFFF
#define INT64_MIN ( -9223372036854775807LL - 1LL ) // 0x8000000000000000

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unsigned Integer types

using uint_8 = unsigned char;
#define UINT_8_MAX ( 255 )              // 0xFF
#define UINT_8_MIN ( 0 )

using uint_16 = unsigned short;
#define UINT_16_MAX ( 65535 )           // 0xFFFF
#define UINT_16_MIN ( 0 )

using uint_32 = unsigned int;
#define UINT_32_MAX ( 4294967295U )     // 0xFFFFFFFF
#define UINT_32_MIN ( 0 )

using uint_64 = unsigned long long;
#define UINT_64_MAX ( 18446744073709551615ULL ) // 0xFFFFFFFFFFFFFFFF
#define UINT_64_MIN ( 0 )

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Floating-point types

using float_32 = float;
#define FLOAT32_MAX ( 3.402823466e+38F )
#define FLOAT32_MIN ( 1.175494351e-38F )
using float_64 = double;
#define FLOAT64_MAX ( 1.7976931348623158e+308 )
#define FLOAT64_MIN ( 2.2250738585072014e-308 )

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pointer and size types

#if ARCH_X86_64
    using signed_size = int_64;
    using unsigned_size = uint_64;
#elif ARCH_X86
    using int_size = int_32;
    using uint_size = uint_32;
#else
    #error "Unsupported architecture"
#endif

INLINE unsigned_size align_up( unsigned_size value, unsigned_size alignment ) { return ( value + ( alignment - 1 ) ) & ~( alignment - 1 ); }
INLINE unsigned_size align_down( unsigned_size value, unsigned_size alignment ) { return value & ~( alignment - 1 ); }
INLINE bool is_aligned( unsigned_size value, unsigned_size alignment ) { return ( value & ( alignment - 1 ) ) == 0; }
INLINE unsigned_size align_up_ptr( void* ptr, unsigned_size alignment ) { return align_up( reinterpret_cast<unsigned_size>( ptr ), alignment ); }
INLINE unsigned_size align_down_ptr( void* ptr, unsigned_size alignment ) { return align_down( reinterpret_cast<unsigned_size>( ptr ), alignment ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calling conventions

#if defined(_MSC_VER)
	#define CDECL __attribute__((cdecl))
	#define SIZE_TYPE ( uint_64 )
#else // defined(__GNUC__) || defined(__clang__)
	#define CDECL
	#define SIZE_TYPE ( long unsigned_size )
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exit codes

#define exit_failed_code ( -1 )
#define exit_success_code ( 0 )

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Boolean values

#define true_value ( 1 )
#define false_value ( 0 )

#define enum_type( name, type ) enum name : type

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// bitwise macros

#define BIT( n ) ( 1ULL << ( n ) )
#define BITMASK( n ) ( ( 1ULL << ( n ) ) - 1ULL )
#define BITFIELD( value, offset, size ) ( ( ( value ) >> ( offset ) ) & BITMASK( size ) )
#define SET_BITFIELD( value, offset, size, new_value ) ( ( value ) = ( ( value ) & ~( BITMASK( size ) << ( offset ) ) ) | ( ( ( new_value ) & BITMASK( size ) ) << ( offset ) ) )
#define SET_BIT( value, bit ) ( ( value ) |= BIT( bit ) )

INLINE bool check_bit( uint_64 value, uint_32 bit ) { return ( value & BIT( bit ) ) != 0; }
INLINE bool check_bitmask( uint_64 value, uint_64 mask ) { return ( value & mask ) == mask; }
INLINE void set_bit( uint_64& value, uint_32 bit ) { value |= BIT( bit ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bytes

#define KILOBYTES( n ) ( ( n ) * 1024ULL )
#define MEGABYTES( n ) ( KILOBYTES( n ) * 1024ULL )
#define GIGABYTES( n ) ( MEGABYTES( n ) * 1024ULL )
#define TERABYTES( n ) ( GIGABYTES( n ) * 1024ULL )

INLINE uint_64 to_kilobytes( const uint_64 bytes ) { return bytes / 1024ULL; }
INLINE uint_64 to_megabytes( const uint_64 bytes ) { return bytes / ( 1024ULL * 1024ULL ); }
INLINE uint_64 to_gigabytes( const uint_64 bytes ) { return bytes / ( 1024ULL * 1024ULL * 1024ULL ); }
INLINE uint_64 to_terabytes( const uint_64 bytes ) { return bytes / ( 1024ULL * 1024ULL * 1024ULL * 1024ULL ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pointer arithmetic

#define PTR_ADD( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) + ( offset ) ) )
#define PTR_SUB( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) - ( offset ) ) )
#define PTR_DIFF( ptr1, ptr2 ) ( reinterpret_cast<unsigned_size>( ptr1 ) - reinterpret_cast<unsigned_size>( ptr2 ) )
#define PTR_OFFSET( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) + ( offset ) ) )
#define PTR_OFFSET_PTR( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) + ( offset ) ) )

INLINE void* ptr_add( void* ptr, unsigned_size offset ) { return reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) + offset ); }
INLINE void* ptr_sub( void* ptr, unsigned_size offset ) { return reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) - offset ); }
INLINE unsigned_size ptr_diff( void* ptr1, void* ptr2 ) { return reinterpret_cast<unsigned_size>( ptr1 ) - reinterpret_cast<unsigned_size>( ptr2 ); }
INLINE void* ptr_offset( void* ptr, unsigned_size offset ) { return reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) + offset ); }
INLINE void* ptr_offset_ptr( void* ptr, unsigned_size offset ) { return reinterpret_cast<void*>( reinterpret_cast<unsigned_size>( ptr ) + offset ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ARRAY_SIZE( x ) ( sizeof( x ) / sizeof( x[0] ) )

#define GLOBAL extern
#define LOCAL static
#define IMPORT

#define UNUSED_VAR( x ) (void)x

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
