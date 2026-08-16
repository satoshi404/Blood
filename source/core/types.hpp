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

using i8 = signed char;
#define I8_MAX ( 127 )              // 0x7F
#define I8_MIN ( -128 )             // 0x80

using i16 = signed short;
#define I16_MAX ( 32767 )           // 0x7FFF
#define I16_MIN ( -32768 )          // 0x8000

using i32 = signed int;
#define I32_MAX ( 2147483647 )      // 0x7FFFFFFF
#define I32_MIN ( -2147483647 - 1 )

using i64 = signed long long;
#define I64_MAX ( 9223372036854775807LL )  // 0x7FFFFFFFFFFFFFFF
#define I64_MIN ( -9223372036854775807LL - 1LL ) // 0x8000000000000000

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Unsigned Integer types

using u8 = unsigned char;
#define U8_MAX ( 255 )              // 0xFF
#define U8_MIN ( 0 )

using u16 = unsigned short;
#define U16_MAX ( 65535 )           // 0xFFFF
#define U16_MIN ( 0 )

using u32 = unsigned int;
#define U32_MAX ( 4294967295U )     // 0xFFFFFFFF
#define U32_MIN ( 0 )

using u64 = unsigned long long;
#define U64_MAX ( 18446744073709551615ULL ) // 0xFFFFFFFFFFFFFFFF
#define U64_MIN ( 0 )

// Type conversion functions ( from )
INLINE u32 u32_from_u64( u64 value ) { return static_cast<u32>( value & 0xFFFFFFFFULL ); }
INLINE u64 u64_from_u32( u32 value ) { return static_cast<u64>( value ); }
INLINE u16 u16_from_u32( u32 value ) { return static_cast<u16>( value & 0xFFFFU ); }
INLINE u32 u32_from_u16( u16 value ) { return static_cast<u32>( value ); }
INLINE u8 u8_from_u32( u32 value ) { return static_cast<u8>( value & 0xFFU ); }
INLINE u32 u32_from_u8( u8 value ) { return static_cast<u32>( value ); }
INLINE u8 u8_from_u64( u64 value ) { return static_cast<u8>( value & 0xFFULL ); }
INLINE u64 u64_from_u8( u8 value ) { return static_cast<u64>( value ); }
INLINE u16 u16_from_u64( u64 value ) { return static_cast<u16>( value & 0xFFFFULL ); }
INLINE u64 u64_from_u16( u16 value ) { return static_cast<u64>( value ); }
INLINE u32 u32_from_i64( i64 value ) { return static_cast<u32>( value & 0xFFFFFFFFLL ); }
// ..

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Floating-point types

using f32 = float;
#define F32_MAX ( 3.402823466e+38F )
#define F32_MIN ( 1.175494351e-38F )
using f64 = double;
#define F64_MAX ( 1.7976931348623158e+308 )
#define F64_MIN ( 2.2250738585072014e-308 )

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pointer and size types

#if ARCH_X86_64
    using isize = i64;
    using usize = u64;
#elif ARCH_X86
    using isize = i32;
    using usize = u32;
#else
    #error "Unsupported architecture"
#endif

INLINE usize align_up( usize value, usize alignment ) { return ( value + ( alignment - 1 ) ) & ~( alignment - 1 ); }
INLINE usize align_down( usize value, usize alignment ) { return value & ~( alignment - 1 ); }
INLINE bool is_aligned( usize value, usize alignment ) { return ( value & ( alignment - 1 ) ) == 0; }
INLINE usize align_up_ptr( void* ptr, usize alignment ) { return align_up( reinterpret_cast<usize>( ptr ), alignment ); }
INLINE usize align_down_ptr( void* ptr, usize alignment ) { return align_down( reinterpret_cast<usize>( ptr ), alignment ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calling conventions

#if defined(_MSC_VER)
	#define CDECL __attribute__((cdecl))
	#define SIZE_TYPE ( u64 )
#else // defined(__GNUC__) || defined(__clang__)
	#define CDECL
	#define SIZE_TYPE ( long usize )
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

INLINE bool check_bit( u64 value, u32 bit ) { return ( value & BIT( bit ) ) != 0; }
INLINE bool check_bitmask( u64 value, u64 mask ) { return ( value & mask ) == mask; }
INLINE void set_bit( u64& value, u32 bit ) { value |= BIT( bit ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bytes

#define KILOBYTES( n ) ( ( n ) * 1024ULL )
#define MEGABYTES( n ) ( KILOBYTES( n ) * 1024ULL )
#define GIGABYTES( n ) ( MEGABYTES( n ) * 1024ULL )
#define TERABYTES( n ) ( GIGABYTES( n ) * 1024ULL )

INLINE u64 bytes_to_kilobytes( u64 bytes ) { return bytes / 1024ULL; }
INLINE u64 bytes_to_megabytes( u64 bytes ) { return bytes / ( 1024ULL * 1024ULL ); }
INLINE u64 bytes_to_gigabytes( u64 bytes ) { return bytes / ( 1024ULL * 1024ULL * 1024ULL ); }
INLINE u64 bytes_to_terabytes( u64 bytes ) { return bytes / ( 1024ULL * 1024ULL * 1024ULL * 1024ULL ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pointer arithmetic

#define PTR_ADD( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) + ( offset ) ) )
#define PTR_SUB( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) - ( offset ) ) )
#define PTR_DIFF( ptr1, ptr2 ) ( reinterpret_cast<usize>( ptr1 ) - reinterpret_cast<usize>( ptr2 ) )
#define PTR_OFFSET( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) + ( offset ) ) )
#define PTR_OFFSET_PTR( ptr, offset ) ( reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) + ( offset ) ) )

INLINE void* ptr_add( void* ptr, usize offset ) { return reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) + offset ); }
INLINE void* ptr_sub( void* ptr, usize offset ) { return reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) - offset ); }
INLINE usize ptr_diff( void* ptr1, void* ptr2 ) { return reinterpret_cast<usize>( ptr1 ) - reinterpret_cast<usize>( ptr2 ); }
INLINE void* ptr_offset( void* ptr, usize offset ) { return reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) + offset ); }
INLINE void* ptr_offset_ptr( void* ptr, usize offset ) { return reinterpret_cast<void*>( reinterpret_cast<usize>( ptr ) + offset ); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ARRAY_LEN( x ) ( sizeof( x ) / sizeof( x[0] ) )

#define GLOBAL
