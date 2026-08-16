#include <core/types.hpp>
#include <core/string.hpp>
#include <core/debug.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

#if PLATFORM_LINUX
    const char* comment = "#";
#elif PLATFORM_WINDOWS
    const char* comment = "@rem";
#endif

#define COMMENT_LENGTH ( 100 )
#define COMMENT( b, length )\
    b.append( comment );\
    b.append( "/", false_value, length );\
    b.append( "\n" );\

#define COMMENT_INTRO_TEXT " Build Generator "
#define COMMENT_INTRO( text, b, l , l2 )\
    COMMENT( b, l2 )\
    b.append( comment, false_value, l );\
    b.append( text );\
    b.append( comment, false_value, l );\
    b.append( "\n" );\
    COMMENT( b, l2 )

const char* toolchain = "clang++";

struct Build
{
    String path;
    const char* work_path;
    const char* obj_path;
};

static const char* files_vendor_x11[] =
{
    "xutil.cpp",
};

static const char* files_core[] =
{
    "debug.cpp",
};

static const char* files_platform[] =
{
    "keyboard.cpp",
    "window.config.cpp",
};


static const char* files_engine[] =
{
    "engine.cpp",
};

static const char* files_sub_platform[] =
{
    "window.cpp",
};

static const char* files_renderer[] =
{
    "gpu.cpp",
};

static const char* files_command[] =
{
    "gpu.command.cpp",
    "gpu.command.dispatch.cpp",
};

static const char* files_pool[] =
{
    "gpu.buffer.pool.cpp",
    "gpu.descriptor.pool.cpp",
    "gpu.mesh.pool.cpp",
    "gpu.texture.pool.cpp",
};

static const char* files_render[] =
{
    "gpu.render.pass.cpp",
    "gpu.render.queue.cpp",
};

static const char* files_resource[] =
{
    "gpu.buffer.cpp",
    "gpu.descriptor.cpp",
    "gpu.material.cpp",
    "gpu.mesh.cpp",
};

static const char* files_state[] =
{
    "gpu.transform.cpp",
};

static const char* files_layer_opengl[] =
{
    "gpu.opengl.cpp",
    "gpu.opengl.buffer.cpp",
    "gpu.opengl.draw.cpp",
    "gpu.opengl.material.cpp",
    "gpu.opengl.mesh.cpp",
    "gpu.opengl.shaders.cpp",
    "gpu.opengl.state.cpp",
    "gpu.opengl.texture.cpp",
};

struct WorkUnit {
    const char* directory;
    const char** files;
    int file_count;
};

static void build( String& generator, Build build, const char* file, const u32 length, const u32 comment_length )
{
    generator.append( "-c ", true_value );

    build.path.append( file );
    generator.append( build.path.data() );

    generator.append( " -o ", true_value );
    generator.append( build.obj_path );
    generator.append( file );
    generator.append( ".o" );
    generator.append( "-Isource", true_value);
}

int main( int argc, char** argv )
{
    {
        String generator;
        generator.init();

        i32 comment_length = static_cast<int>( COMMENT_LENGTH );
        if ( comment_length <= 0 ) comment_length = 1;

        i32 length = static_cast<int>( comment_length / 2 ) - static_cast<int>( strlen(COMMENT_INTRO_TEXT) / 2 );
        if ( length <= 0 ) length = 1;

        COMMENT_INTRO( COMMENT_INTRO_TEXT, generator, length, comment_length );

        generator.append( "\n", false_value, 2 );
        #if PLATFORM_LINUX
            generator.append( "rm build/objects/*.o" );
        #endif
        generator.append( "\n", false_value, 2 );

        const char* ext = NULL;

        #if PLATFORM_LINUX
            ext = ".sh";

            generator.append( "#!/bin/bash" );
            generator.append( "\n" );
            generator.append( "set -xe" );
            generator.append( "\n", false_value, 2 );
        #elif PLATFORM_WINDOWS
            ext = ".bat";
        #endif

      //  generator.append( toolchain );

        WorkUnit units[] =
        {
            { "core/", files_core, static_cast<int>( ARRAY_LEN( files_core ) ) },
            { "platform/", files_platform, static_cast<int>( ARRAY_LEN( files_platform ) ) },
            // Sub platform
            #if PLATFORM_LINUX
                { "vendor/x11/", files_vendor_x11, static_cast<int>( ARRAY_LEN( files_vendor_x11 ) ) },
                { "platform/xcb/", files_sub_platform, static_cast<int>( ARRAY_LEN( files_sub_platform ) ) },
            #elif PLATFORM_WINDOWS
                { "platform/windows/", files_sub_platform, static_cast<int>( ARRAY_LEN( files_sub_platform ) ) },
            #endif
            {"renderer/gpu/", files_renderer, static_cast<int>( ARRAY_LEN( files_renderer ) ) },
            {"renderer/gpu/command/", files_command, static_cast<int>( ARRAY_LEN( files_command ) ) },
            {"renderer/gpu/pool/", files_pool, static_cast<int>( ARRAY_LEN( files_pool ) ) },
            {"renderer/gpu/render/", files_render, static_cast<int>( ARRAY_LEN( files_render ) ) },
            {"renderer/gpu/resource/", files_resource, static_cast<int>( ARRAY_LEN( files_resource ) ) },
            {"renderer/gpu/state/", files_state, static_cast<int>( ARRAY_LEN( files_state ) ) },
            #if API_OPENGL
                {"renderer/layer/opengl/", files_layer_opengl, static_cast<int>( ARRAY_LEN( files_layer_opengl ) ) },
            #endif
            {"engine/", files_engine, static_cast<int>( ARRAY_LEN( files_engine ) ) },
        };

        int unit_count = static_cast<int>( sizeof(units) / sizeof(units[0]) );

        for ( int w = 0; w < unit_count; w++ )
        {
            WorkUnit current_unit = units[ w ];

            for ( int i = 0; i < current_unit.file_count; i++ )
            {
                const char* file =  current_unit.files[ i ];

                Build b;

                b.path.init( "source/" );
                b.obj_path = "build/objects/";
                b.work_path = current_unit.directory;
                b.path.append( b.work_path );

                if ( i == 0 )
                {
                char text[256];
                sprintf( text, "[ %s ]", current_unit.directory );

                length = static_cast<int>( comment_length / 2 ) - static_cast<int>( strlen( text ) / 2 );
                if ( length <= 0 ) length = 1;

                COMMENT_INTRO( text, generator, length, comment_length );
                }

                String temp;
                temp.init( toolchain );
                build( temp, b,file, length, comment_length );

                generator.append( temp.data() );
                generator.append( "\n" );

            }
            COMMENT( generator, comment_length );
            generator.append( "\n", false_value, 2 );
        }

        Debug::Println( PrintColor_Yellow, generator.data() );

        {
            String script;
            script.init( ".blood/vscblood/generator/" );
            script.append( "build" );
            script.append( ext );
            generator.save_data( script.data() );
        }
    }

    return exit_success_code;
}
