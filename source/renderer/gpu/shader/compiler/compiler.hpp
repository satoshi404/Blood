#pragma once

#include <core/types.hpp>

enum_type( Blood_CompilerKind, u32 )
{
	Blood_CompilerKind_Eof = 256,
	Blood_CompilerKind_Identifier,
	Blood_CompilerKind_Literal_Int,
	Blood_CompilerKind_Literal_Char,

	Blood_CompilerKind_Count,
};

extern const char* kinds[ Blood_CompilerKind_Count ];