#pragma once

#include <pipeline.hpp>

// TODO: Better implementation

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// stdint.h

typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef short              int16_t;
typedef unsigned short     uint16_t;
typedef int                int32_t;
typedef unsigned int       uint32_t;

/* 64-bit types */
#if ARCH_X86_64
	#if PLATFORM_WINDOWS
		typedef long long  int64_t;
		typedef unsigned long long uint64_t;
	#else
		typedef long int64_t;
		typedef unsigned long uint64_t;
	#endif
#else
	typedef long long int64_t;
	typedef unsigned long long uint64_t;
#endif

/* Pointer-holding integer types */
#if ARCH_X86_64
	#if PLATFORM_WINDOWS
		typedef long long intptr_t;
		typedef unsigned long long uintptr_t;
	#else
		typedef long intptr_t;
		typedef unsigned long uintptr_t;
	#endif
#else
	typedef int intptr_t;
	typedef unsigned int uintptr_t;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////