#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Platform

#if defined(__linux__)
	#define PLATFORM_LINUX   ( 1 )
	#define PLATFORM_WINDOWS ( 0 )
#elif defined(__WIN32) || defined(__WIN64)
	#define PLATFORM_WINDOWS ( 1 )
	#define PLATFORM_LINUX   ( 0 )
#else
	#error "Platform not supported"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Architectures

/* 64-bit types vary between 32-bit and 64-bit OS architectures */
#if defined(__x86_64__) || defined(__LP64__) || defined(_WIN64) || defined(_M_X64) || defined(__aarch64__)
	#define ARCH_X86_64 ( 1 )
	#define ARCH_X86    ( 0 )
#elif defined(__i386__) || defined(_X86_) || defined(_M_IX86) || defined(__arm__)
	#define ARCH_X86_64 ( 0 )
	#define ARCH_X86    ( 1 )
#else
	#error "Unsupported architecture"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Toolchain

#if defined(__GNUC__) || defined(__clang__)
	#define COMPILER_GCC ( 1 )
	#define COMPILER_MSVC ( 0 )
#elif defined(_MSC_VER)
	#define COMPILER_GCC ( 0 )
	#define COMPILER_MSVC ( 1 )
#else
	#error "Unsupported compiler"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////