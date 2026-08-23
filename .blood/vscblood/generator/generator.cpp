#include <core/filesystem.hpp>
#include <core/string.hpp>
#include <core/debug.hpp>
#include <core/types.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

// TODO:
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#if PLATFORM_LINUX
    const char* comment = "#";
#elif PLATFORM_WINDOWS
    const char* comment = "@rem";
#endif

#define SECTION_LENGTH ( 100 )
#define SECTION( string, length ) string\
		.append( comment )\
        .append( "/", false_value, length )\
        .append( "\n" );\

#define SECTION_COMMENT( text, string, length , section_length )\
    SECTION( string, section_length ) string\
		.append( comment, false_value, length )\
        .append( text )\
        .append( comment, false_value, length )\
        .append( "\n" );\
    SECTION( string, section_length )

const char* toolchain_cxx = "clang++";
const char* build_dir = "build";
const char* obj_dir = "build/objects";
const char* bin_dir = "build/bin";

static const char* engine_dirs[] =
{
	"source/core",
	"source/platform",
	"source/platform/xcb",
	"source/platform/windows",
	"source/vendor/x11",
	"source/renderer/gpu",
	"source/renderer/gpu/command",
	"source/renderer/gpu/pool",
	"source/renderer/gpu/render",
	"source/renderer/gpu/resource",
	"source/renderer/gpu/state",
	"source/renderer/layer/opengl",
	"source/engine",
};

static const char* runtime_dirs[] =
{
	"runtime/flappy",
	"runtime/flappy/scripts"
};

// Helpers
static bool ends_with( const char* str, const char* suffix )
{
	size_t len = strlen( str );
	size_t suflen = strlen( suffix );

	if ( suflen > len ) return false_value;

	return strcmp( str + len - suflen, suffix ) == 0;
}

static void collect_cpp_files( const char* dir, String& collect, bool& found_any )
{
	DIR* d =  opendir( dir );
	if ( !d ) return;

	struct dirent* entry;
	while ( (entry = readdir( d)) != nullptr )
	{
		if ( entry->d_type == DT_REG && ends_with( entry->d_name, ".cpp" ) )
		{
			collect
				.append( " " )
				.append( dir )
				.append( "/" )
				.append( entry->d_name );
			found_any = true_value;
		}
	}
	closedir( d );
}

static void write_rule_cxx( String& out )
{
	out.append( "rule cxx\n" );
	out.append( " command = " );
	out.append( toolchain_cxx );
	out.append( " -MMD -MT $out -MF $out.d " );
	out.append( " $cflags -c $in -o $out\n" );
	out.append( " description = CXX $in\n" );
	out.append( " depfile = $out.d\n" );
	out.append( " deps = gcc\n\n" );
}

static void write_rule_link( String& out )
{
	out
		.append( "rule link\n" )
		.append( " command = " )
		.append( toolchain_cxx )
		.append( " $in $ldflags -o $out\n" )
		.append( " description = LINK $out\n\n" );
}

static void write_rule_archive( String& out )
{
	out
		.append( "rule archive\n" )
		.append( " command = ar rcs $out $in\n" )
		.append( " description = AR $out\n\n" );
}

//
// TODO: Parse args
//
int main( int argc, char** argv )
{
	String ninja;
	ninja.init();

	int_32 section_len = SECTION_LENGTH;
	int_32 length = ( section_len / 2 ) - (int)( strlen( "Blood Build Generator ( Ninja )" ) / 2 );
	if ( section_len <= UINT_32_MIN ) length = 1;

	SECTION_COMMENT( "Blood Build Generator ( Ninja )", ninja, length, section_len );
	ninja.append( "\n\n");

	// Header
	ninja.append( "ninja_required_version = 1.10\n\n" );
	ninja.append( "builddir = " );
	ninja.append( build_dir );
	ninja.append( "\n" );
	ninja.append( "objdir = " );
	ninja.append( obj_dir );
	ninja.append( "\n" );
	ninja.append( "bindir = " );
	ninja.append( bin_dir );
	ninja.append( "\n\n" );

	ninja.append( "cflags = -std=c++20 -Isource -Wall -Wextra -Wno-unused-parameter -g -O0\n" );

#if PLATFORM_LINUX
	ninja.append( "ldflags = -lxcb -lxcb-keysyms -lX11 -lX11-xcb -lGL -lGLEW -lpthread -lm\n\n" );
#elif PLATFORM_WINDOWS
	ninja.append( "ldflags =\n\n" );
#endif

	write_rule_cxx( ninja );
	write_rule_link( ninja );
	write_rule_archive( ninja );

	// Collect files
	String engine_objects;
	engine_objects.init();
	String runtime_objects;
	runtime_objects.init();

	uint_32 index = 0;

	// Engine
	for ( uint_32 index = 0; index < ARRAY_SIZE( engine_dirs ); index++ )
	{
		const char* path_dir = engine_dirs[ index ];

		// Skip folders
		bool skip = false_value;
#if PLATFORM_LINUX
		skip = strstr( path_dir, "windows" ) != nullptr;
#elif PLATFORM_WINDOWS
		skip = strstr( path_dir, "xcb" ) != nullptr || strstr( path_dir, "x11" ) != nullptr;
#endif
		if ( skip ) continue;

// Vulkan || D3D12
#if !API_OPENGL
		skip = strstr( path_dir, "opengl" ) != nullptr;
#endif
		if ( skip ) continue;

		String files;
		files.init();
		bool found = false_value;
		collect_cpp_files( path_dir, files, found );

		if ( !found ) continue;

		{
			char text[256];
			sprintf( text, "[ %s ]", path_dir );

			length = ( section_len / 2 ) - (int)( strlen( text ) / 2);
			if ( length <= UINT_32_MIN ) length = 1;
			SECTION_COMMENT( text, ninja, length, section_len );
		}

		DIR* d;
		if ( (d = opendir( path_dir ) ) != nullptr )
		{
			struct dirent* entry;
			while ( (entry = readdir( d ) ) != nullptr )
			{
				if ( (entry->d_type == DT_REG && ends_with( entry->d_name, ".cpp" )) )
				{
					String source;
					source.init( path_dir );
					source.append( "/" );
					source.append( entry->d_name );

					String obj;
					obj.init( "$objdir/" );

					// Compile same name files
					char object[256] = {};

					sprintf( object, "index.%u_name.", index  );

					obj.append( object );
					obj.append( entry->d_name );
					obj.append( ".o" );

					ninja.append( "build " );
					ninja.append( obj.data() );
					ninja.append( ": cxx " );
					ninja.append( source.data() );
					ninja.append( "\n" );

					engine_objects.append( " " );
					engine_objects.append( obj.data() );
				}
			}

			closedir( d );
		}

		SECTION( ninja, section_len );
		ninja.append( "\n" );
	}

	// Runtime
	for ( uint_32 index = 0; index < ARRAY_SIZE( runtime_dirs ); index++ )
	{
		const char* path_dir = runtime_dirs[ index ];

		{
			char text[256];
			sprintf( text, "[ %s ]", path_dir );

			length = ( section_len / 2 ) - (int)( strlen( text ) / 2);
			if ( length <= UINT_32_MIN ) length = 1;
			SECTION_COMMENT( text, ninja, length, section_len );
		}

		DIR* d;
		if ( (d = opendir( path_dir ) ) != nullptr )
		{
			struct dirent* entry;
			while ( (entry = readdir( d ) ) != nullptr )
			{
				if ( (entry->d_type == DT_REG && ends_with( entry->d_name, ".cpp" )) )
				{
					String source;
					source.init( path_dir );
					source.append( "/" );
					source.append( entry->d_name );

					String obj;
					obj.init( "$objdir/" );
					obj.append( entry->d_name );
					obj.append( ".o" );

					ninja.append( "build " );
					ninja.append( obj.data() );
					ninja.append( ": cxx " );
					ninja.append( source.data() );
					ninja.append( "\n" );

					runtime_objects.append( " " );
					runtime_objects.append( obj.data() );
				}
			}

			closedir( d );
		}

		SECTION( ninja, section_len );
		ninja.append( "\n" );
	}

	// Targets
	SECTION_COMMENT( "Targets", ninja, length, section_len )

	ninja.append( "build $bindir/libblood.a: archive" );
	ninja.append( engine_objects.data() );
	ninja.append( "\n\n" );

	ninja.append( "build $bindir/flappy: link" );
	ninja.append( runtime_objects.data() );
	ninja.append( engine_objects.data() );
	ninja.append( "\n\n" );

	ninja.append( "default $bindir/flappy\n\n" );

	// Save
	{
		String path;
		path.init( "build.ninja" );
		ninja.save_data( path.data() );
		Debug::Println( PrintColorType_Green, "Generated: %s", path.data() );
	}

	// Helper save
	{
		String helper;
		helper.init();
#if PLATFORM_LINUX
		helper.append( "#!/bin/bash\nset -e\n" );
		helper.append( "mkdir -p build/objects build/bin\n" );
		helper.append( "ninja -C build \"$@\"\n" );
		helper.append( "build.sh" );
#else
		helper.append( "@echo off" );
		helper.append( "if not exists build\\objects mkdir build\\objects\n" );
		helper.append( "if not exists build\\bin mkdir build\\bin\n" );
		helper.append( "ninja -C build %*\n" );
		helper.append( "build.bat" );
#endif

		Debug::Println( PrintColorType_Cyan, "Generated helper script" );
	}

	Debug::Println( PrintColorType_Yellow, "Blood Ninja Generator finished ( auto-scan )" );

	return exit_success_code;
}