#include <engine/system/serializer.hpp>
#include <engine/system/node.hpp>
#include <engine/system/component.hpp>

#include <core/string.hpp>
#include <core/debug.hpp>
#include <core/memory.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helpers

namespace
{
    void append_indent( String& out, int_32 indent )
    {
        for ( int_32 i = 0; i < indent; ++i )
            out.append( "    " );
    }

    void append_f( String& out, const char* fmt, ... )
    {
        char buf[ 512 ];
        va_list args;
        va_start( args, fmt );
        vsnprintf( buf, sizeof( buf ), fmt, args );
        va_end( args );
        out.append( buf );
    }

    // ------------------------------------------------------------------
    // Simple tokenizer / parser state

    struct Parser
    {
        const char* data   = nullptr;
        uint_64     size   = 0;
        uint_64     pos    = 0;
        int_32      line   = 1;

        void init( const char* src, uint_64 len )
        {
            data = src;
            size = len;
            pos  = 0;
            line = 1;
        }

        bool eof() const { return pos >= size; }

        char peek() const
        {
            if ( eof() ) return '\0';
            return data[ pos ];
        }

        char next()
        {
            if ( eof() ) return '\0';
            char c = data[ pos++ ];
            if ( c == '\n' ) ++line;
            return c;
        }

        void skip_whitespace()
        {
            while ( !eof() )
            {
                char c = peek();
                if ( c == ' ' || c == '\t' || c == '\r' || c == '\n' )
                {
                    next();
                    continue;
                }
                // comments //
                if ( c == '/' && pos + 1 < size && data[ pos + 1 ] == '/' )
                {
                    while ( !eof() && peek() != '\n' )
                        next();
                    continue;
                }
                break;
            }
        }

        bool expect( char c )
        {
            skip_whitespace();
            if ( peek() == c )
            {
                next();
                return true_value;
            }
            return false_value;
        }

        bool match_keyword( const char* kw )
        {
            skip_whitespace();
            uint_64 len = strlen( kw );
            if ( pos + len > size )
                return false_value;

            for ( uint_64 i = 0; i < len; ++i )
            {
                if ( data[ pos + i ] != kw[ i ] )
                    return false_value;
            }

            // must be followed by non-identifier char
            if ( pos + len < size )
            {
                char after = data[ pos + len ];
                if ( isalnum( (unsigned char)after ) || after == '_' )
                    return false_value;
            }

            pos += len;
            return true_value;
        }

        bool read_identifier( char* out, uint_32 max_len )
        {
            skip_whitespace();
            uint_32 i = 0;

            char c = peek();
            if ( !( isalpha( (unsigned char)c ) || c == '_' ) )
                return false_value;

            while ( !eof() && i < max_len - 1 )
            {
                c = peek();
                if ( isalnum( (unsigned char)c ) || c == '_' || c == '.' || c == '-' )
                {
                    out[ i++ ] = next();
                }
                else break;
            }
            out[ i ] = '\0';
            return i > 0;
        }

        bool read_string( char* out, uint_32 max_len )
        {
            skip_whitespace();
            if ( peek() != '"' )
                return false_value;
            next(); // "

            uint_32 i = 0;
            while ( !eof() && i < max_len - 1 )
            {
                char c = peek();
                if ( c == '"' )
                {
                    next();
                    out[ i ] = '\0';
                    return true_value;
                }
                if ( c == '\\' )
                {
                    next();
                    if ( !eof() )
                        out[ i++ ] = next();
                    continue;
                }
                out[ i++ ] = next();
            }
            out[ i ] = '\0';
            return false_value;
        }

        bool read_float( float_32& value )
        {
            skip_whitespace();
            char buf[ 64 ];
            uint_32 i = 0;

            char c = peek();
            if ( c == '-' || c == '+' )
                buf[ i++ ] = next();

            bool has_digit = false_value;
            while ( !eof() && i < 63 )
            {
                c = peek();
                if ( isdigit( (unsigned char)c ) || c == '.' || c == 'e' || c == 'E' )
                {
                    buf[ i++ ] = next();
                    has_digit = true_value;
                }
                else break;
            }
            buf[ i ] = '\0';

            if ( !has_digit )
                return false_value;

            value = static_cast<float_32>( atof( buf ) );
            return true_value;
        }

        bool read_int( int_32& value )
        {
            float_32 f = 0.f;
            if ( !read_float( f ) )
                return false_value;
            value = static_cast<int_32>( f );
            return true_value;
        }

        bool read_bool( bool& value )
        {
            skip_whitespace();
            if ( match_keyword( "true" ) || match_keyword( "True" ) || match_keyword( "1" ) )
            {
                value = true_value;
                return true_value;
            }
            if ( match_keyword( "false" ) || match_keyword( "False" ) || match_keyword( "0" ) )
            {
                value = false_value;
                return true_value;
            }
            return false_value;
        }

        bool read_vec3( float_32& x, float_32& y, float_32& z )
        {
            if ( !read_float( x ) ) return false_value;
            if ( !read_float( y ) ) return false_value;
            if ( !read_float( z ) ) return false_value;
            return true_value;
        }
    };

    // ------------------------------------------------------------------
    // Write helpers

    bool write_node( NodeHandle handle, String& out, int_32 indent )
    {
        Node* node = NodeSystem::get( handle );
        if ( !node )
            return false_value;

        append_indent( out, indent );
        append_f( out, "node \"%s\"\n", node->name );
        append_indent( out, indent );
        out.append( "{\n" );

        // basic props
        append_indent( out, indent + 1 );
        append_f( out, "active = %s\n", node->active ? "true" : "false" );
        append_indent( out, indent + 1 );
        append_f( out, "visible = %s\n", node->visible ? "true" : "false" );

        // transform
        append_indent( out, indent + 1 );
        append_f( out, "position = %.6f %.6f %.6f\n",
            node->local.position.x, node->local.position.y, node->local.position.z );
        append_indent( out, indent + 1 );
        append_f( out, "rotation = %.6f %.6f %.6f\n",
            node->local.rotation.x, node->local.rotation.y, node->local.rotation.z );
        append_indent( out, indent + 1 );
        append_f( out, "scale = %.6f %.6f %.6f\n",
            node->local.scale.x, node->local.scale.y, node->local.scale.z );

        // components
        for ( uint_64 i = 0; i < node->components.size(); ++i )
        {
            ComponentHandle ch = node->components[ i ];
            Component* base = ComponentSystem::get_base( ch );
            if ( !base )
                continue;

            append_indent( out, indent + 1 );

            switch ( base->type )
            {
                case ComponentType_Render:
                {
                    RenderComponent* r = ComponentSystem::get_render( ch );
                    out.append( "component Render\n" );
                    append_indent( out, indent + 1 );
                    out.append( "{\n" );
                    if ( r )
                    {
                        append_indent( out, indent + 2 );
                        append_f( out, "layer = %d\n", r->layer );
                        append_indent( out, indent + 2 );
                        append_f( out, "z_order = %.6f\n", r->z_order );
                        append_indent( out, indent + 2 );
                        append_f( out, "cast_shadow = %s\n", r->cast_shadow ? "true" : "false" );
                        append_indent( out, indent + 2 );
                        append_f( out, "receive_shadow = %s\n", r->receive_shadow ? "true" : "false" );
                    }
                    append_indent( out, indent + 1 );
                    out.append( "}\n" );
                    break;
                }
                case ComponentType_Camera:
                {
                    CameraComponent* c = ComponentSystem::get_camera( ch );
                    out.append( "component Camera\n" );
                    append_indent( out, indent + 1 );
                    out.append( "{\n" );
                    if ( c )
                    {
                        append_indent( out, indent + 2 );
                        append_f( out, "fov = %.6f\n", c->fov );
                        append_indent( out, indent + 2 );
                        append_f( out, "near = %.6f\n", c->near_plane );
                        append_indent( out, indent + 2 );
                        append_f( out, "far = %.6f\n", c->far_plane );
                        append_indent( out, indent + 2 );
                        append_f( out, "orthographic = %s\n", c->is_orthographic ? "true" : "false" );
                        append_indent( out, indent + 2 );
                        append_f( out, "ortho_size = %.6f\n", c->ortho_size );
                    }
                    append_indent( out, indent + 1 );
                    out.append( "}\n" );
                    break;
                }
                case ComponentType_Light:
                {
                    LightComponent* l = ComponentSystem::get_light( ch );
                    out.append( "component Light\n" );
                    append_indent( out, indent + 1 );
                    out.append( "{\n" );
                    if ( l )
                    {
                        append_indent( out, indent + 2 );
                        append_f( out, "intensity = %.6f\n", l->intensity );
                        append_indent( out, indent + 2 );
                        append_f( out, "range = %.6f\n", l->range );
                        append_indent( out, indent + 2 );
                        append_f( out, "type = %u\n", l->light_type );
                    }
                    append_indent( out, indent + 1 );
                    out.append( "}\n" );
                    break;
                }
                case ComponentType_Mesh:
                {
                    out.append( "component Mesh\n" );
                    append_indent( out, indent + 1 );
                    out.append( "{\n" );
                    // MeshHandle is runtime-only for now; path support later
                    append_indent( out, indent + 1 );
                    out.append( "}\n" );
                    break;
                }
                case ComponentType_Material:
                {
                    out.append( "component Material\n" );
                    append_indent( out, indent + 1 );
                    out.append( "{\n" );
                    append_indent( out, indent + 1 );
                    out.append( "}\n" );
                    break;
                }
                case ComponentType_Script:
                {
                    out.append( "component Script\n" );
                    append_indent( out, indent + 1 );
                    out.append( "{\n" );
                    // scripts are code-side for now
                    append_indent( out, indent + 1 );
                    out.append( "}\n" );
                    break;
                }
                default:
                    break;
            }
        }

        // children
        for ( uint_64 i = 0; i < node->children.size(); ++i )
        {
            write_node( node->children[ i ], out, indent + 1 );
        }

        append_indent( out, indent );
        out.append( "}\n" );
        return true_value;
    }

    // ------------------------------------------------------------------
    // Parse a node block (assumes "node" keyword already consumed)

    bool parse_component_block( Parser& p, NodeHandle owner )
    {
        char type_name[ 64 ] = {};
        if ( !p.read_identifier( type_name, 64 ) )
        {
            // try quoted
            if ( !p.read_string( type_name, 64 ) )
                return false_value;
        }

        if ( !p.expect( '{' ) )
            return false_value;

        ComponentType ctype = ComponentType_None;
        if ( strcmp( type_name, "Render" ) == 0 )      ctype = ComponentType_Render;
        else if ( strcmp( type_name, "Camera" ) == 0 ) ctype = ComponentType_Camera;
        else if ( strcmp( type_name, "Light" ) == 0 )  ctype = ComponentType_Light;
        else if ( strcmp( type_name, "Mesh" ) == 0 )   ctype = ComponentType_Mesh;
        else if ( strcmp( type_name, "Material" ) == 0 ) ctype = ComponentType_Material;
        else if ( strcmp( type_name, "Script" ) == 0 ) ctype = ComponentType_Script;

        ComponentHandle ch = {};
        if ( ctype != ComponentType_None )
            ch = ComponentSystem::add( owner, ctype );

        // read properties until }
        while ( !p.eof() )
        {
            p.skip_whitespace();
            if ( p.peek() == '}' )
            {
                p.next();
                break;
            }

            char key[ 64 ] = {};
            if ( !p.read_identifier( key, 64 ) )
            {
                p.next(); // skip unknown
                continue;
            }

            if ( !p.expect( '=' ) )
                continue;

            if ( ctype == ComponentType_Render )
            {
                RenderComponent* r = ComponentSystem::get_render( ch );
                if ( r )
                {
                    if ( strcmp( key, "layer" ) == 0 ) { int_32 v; if ( p.read_int( v ) ) r->layer = v; }
                    else if ( strcmp( key, "z_order" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) r->z_order = v; }
                    else if ( strcmp( key, "cast_shadow" ) == 0 ) { bool v; if ( p.read_bool( v ) ) r->cast_shadow = v; }
                    else if ( strcmp( key, "receive_shadow" ) == 0 ) { bool v; if ( p.read_bool( v ) ) r->receive_shadow = v; }
                }
            }
            else if ( ctype == ComponentType_Camera )
            {
                CameraComponent* c = ComponentSystem::get_camera( ch );
                if ( c )
                {
                    if ( strcmp( key, "fov" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) c->fov = v; }
                    else if ( strcmp( key, "near" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) c->near_plane = v; }
                    else if ( strcmp( key, "far" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) c->far_plane = v; }
                    else if ( strcmp( key, "orthographic" ) == 0 ) { bool v; if ( p.read_bool( v ) ) c->is_orthographic = v; }
                    else if ( strcmp( key, "ortho_size" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) c->ortho_size = v; }
                }
            }
            else if ( ctype == ComponentType_Light )
            {
                LightComponent* l = ComponentSystem::get_light( ch );
                if ( l )
                {
                    if ( strcmp( key, "intensity" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) l->intensity = v; }
                    else if ( strcmp( key, "range" ) == 0 ) { float_32 v; if ( p.read_float( v ) ) l->range = v; }
                    else if ( strcmp( key, "type" ) == 0 ) { int_32 v; if ( p.read_int( v ) ) l->light_type = (uint_32)v; }
                }
            }
            else
            {
                // skip value
                float_32 dummy;
                bool bdummy;
                char sdummy[ 64 ];
                if ( !p.read_float( dummy ) && !p.read_bool( bdummy ) && !p.read_string( sdummy, 64 ) )
                    p.next();
            }
        }

        return true_value;
    }

    bool parse_node_block( Parser& p, NodeHandle parent, Scene& scene )
    {
        char name[ 64 ] = {};
        if ( !p.read_string( name, 64 ) )
        {
            // allow unquoted identifier
            if ( !p.read_identifier( name, 64 ) )
                return false_value;
        }

        if ( !p.expect( '{' ) )
            return false_value;

        NodeHandle handle = {};
        if ( parent.is_valid() )
            handle = scene.create_child( parent, name );
        else
            handle = scene.create_node( name );

        if ( !handle.is_valid() )
            return false_value;

        Node* node = NodeSystem::get( handle );
        if ( !node )
            return false_value;

        while ( !p.eof() )
        {
            p.skip_whitespace();
            if ( p.peek() == '}' )
            {
                p.next();
                break;
            }

            // nested node
            if ( p.match_keyword( "node" ) )
            {
                if ( !parse_node_block( p, handle, scene ) )
                    return false_value;
                continue;
            }

            // component
            if ( p.match_keyword( "component" ) )
            {
                if ( !parse_component_block( p, handle ) )
                    return false_value;
                continue;
            }

            // property
            char key[ 64 ] = {};
            if ( !p.read_identifier( key, 64 ) )
            {
                p.next();
                continue;
            }

            if ( !p.expect( '=' ) )
                continue;

            if ( strcmp( key, "active" ) == 0 )
            {
                bool v;
                if ( p.read_bool( v ) ) node->active = v;
            }
            else if ( strcmp( key, "visible" ) == 0 )
            {
                bool v;
                if ( p.read_bool( v ) ) node->visible = v;
            }
            else if ( strcmp( key, "position" ) == 0 )
            {
                float_32 x, y, z;
                if ( p.read_vec3( x, y, z ) )
                    NodeSystem::set_local_position( handle, x, y, z );
            }
            else if ( strcmp( key, "rotation" ) == 0 )
            {
                float_32 x, y, z;
                if ( p.read_vec3( x, y, z ) )
                    NodeSystem::set_local_rotation( handle, x, y, z );
            }
            else if ( strcmp( key, "scale" ) == 0 )
            {
                float_32 x, y, z;
                if ( p.read_vec3( x, y, z ) )
                    NodeSystem::set_local_scale( handle, x, y, z );
            }
            else
            {
                // unknown property - try skip value
                float_32 f;
                bool b;
                char s[ 64 ];
                if ( !p.read_float( f ) && !p.read_bool( b ) && !p.read_string( s, 64 ) )
                    p.next();
            }
        }

        return true_value;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SceneSerializer::save( const Scene& scene, const char* path )
{
    if ( !path || !scene.root.is_valid() )
    {
        Debug::Println( PrintColorType_Red, "[SceneSerializer] save: invalid scene or path" );
        return false_value;
    }

    String out;
    out.init( "" );

    out.append( "// Blood Engine Scene File\n" );
    out.append( "// Auto-generated - edit with care\n\n" );

    append_f( out, "scene \"%s\"\n", scene.name[0] ? scene.name : "Untitled" );
    out.append( "{\n" );
    append_indent( out, 1 );
    out.append( "version = 1\n\n" );

    // Write all direct children of root (root itself is the scene container)
    Node* root = NodeSystem::get( scene.root );
    if ( root )
    {
        for ( uint_64 i = 0; i < root->children.size(); ++i )
        {
            write_node( root->children[ i ], out, 1 );
            out.append( "\n" );
        }
    }

    out.append( "}\n" );

    if ( !out.save_data( path ) )
    {
        Debug::Println( PrintColorType_Red, "[SceneSerializer] failed to write: %s", path );
        return false_value;
    }

    Debug::Println( PrintColorType_Green, "[SceneSerializer] saved: %s", path );
    return true_value;
}

bool SceneSerializer::load( Scene& scene, const char* path )
{
    if ( !path )
    {
        Debug::Println( PrintColorType_Red, "[SceneSerializer] load: null path" );
        return false_value;
    }

    FILE* fp = fopen( path, "rb" );
    if ( !fp )
    {
        Debug::Println( PrintColorType_Red, "[SceneSerializer] cannot open: %s", path );
        return false_value;
    }

    fseek( fp, 0, SEEK_END );
    long file_size = ftell( fp );
    fseek( fp, 0, SEEK_SET );

    if ( file_size <= 0 || file_size > 16 * 1024 * 1024 ) // 16 MB limit
    {
        fclose( fp );
        Debug::Println( PrintColorType_Red, "[SceneSerializer] invalid file size" );
        return false_value;
    }

    char* buffer = static_cast<char*>( malloc( file_size + 1 ) );
    if ( !buffer )
    {
        fclose( fp );
        return false_value;
    }

    size_t read = fread( buffer, 1, file_size, fp );
    fclose( fp );
    buffer[ read ] = '\0';

    // Clear existing children of root
    if ( scene.root.is_valid() )
    {
        Node* root = NodeSystem::get( scene.root );
        if ( root )
        {
            // Destroy children (and their components)
            while ( root->children.size() > 0 )
            {
                NodeHandle child = root->children[ 0 ];
                NodeSystem::destroy( child );
            }
        }
    }

    Parser p;
    p.init( buffer, read );

    p.skip_whitespace();
    if ( !p.match_keyword( "scene" ) )
    {
        Debug::Println( PrintColorType_Red, "[SceneSerializer] expected 'scene' keyword (line %d)", p.line );
        free( buffer );
        return false_value;
    }

    char scene_name[ 64 ] = {};
    if ( !p.read_string( scene_name, 64 ) )
        p.read_identifier( scene_name, 64 );

    if ( scene_name[0] )
    {
        // copy name
        uint_32 i = 0;
        while ( scene_name[i] && i < 63 )
        {
            scene.name[i] = scene_name[i];
            ++i;
        }
        scene.name[i] = '\0';
    }

    if ( !p.expect( '{' ) )
    {
        Debug::Println( PrintColorType_Red, "[SceneSerializer] expected '{' after scene name" );
        free( buffer );
        return false_value;
    }

    while ( !p.eof() )
    {
        p.skip_whitespace();
        if ( p.peek() == '}' )
        {
            p.next();
            break;
        }

        if ( p.match_keyword( "version" ) )
        {
            p.expect( '=' );
            int_32 ver = 0;
            p.read_int( ver );
            continue;
        }

        if ( p.match_keyword( "node" ) )
        {
            // top-level nodes become children of scene.root
            if ( !parse_node_block( p, scene.root, scene ) )
            {
                Debug::Println( PrintColorType_Red, "[SceneSerializer] failed to parse node (line %d)", p.line );
                free( buffer );
                return false_value;
            }
            continue;
        }

        // skip unknown
        char dummy[ 64 ];
        if ( !p.read_identifier( dummy, 64 ) )
            p.next();
        else if ( p.expect( '=' ) )
        {
            float_32 f; bool b; char s[64];
            if ( !p.read_float( f ) && !p.read_bool( b ) && !p.read_string( s, 64 ) )
                p.next();
        }
    }

    free( buffer );

    scene.update_transforms();

    Debug::Println( PrintColorType_Green, "[SceneSerializer] loaded: %s (nodes: %llu, components: %llu)",
        path, NodeSystem::count(), ComponentSystem::count() );

    return true_value;
}

bool SceneSerializer::save_node_recursive( NodeHandle node, String& out, int_32 indent )
{
    return write_node( node, out, indent );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////