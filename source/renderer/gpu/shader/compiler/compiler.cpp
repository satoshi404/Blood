#include <renderer/gpu/shader/compiler/compiler.hpp>

struct MapKind
{
	const char* name;
	uint_32 kind;
};

const MapKind kinds[ Blood_CompilerKind_Count ] =
{
	{ "Eof", Blood_CompilerKind_Eof },
	{ "Identifier", Blood_CompilerKind_Identifier },
	{ "Literal Integer", Blood_CompilerKind_Literal_Int },
	{ "Literal Char", Blood_CompilerKind_Literal_Char },
};

// ..