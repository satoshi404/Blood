#include <engine/engine.hpp>
#include <engine/system/node.hpp>
#include <platform/keyboard.hpp>
#include <platform/window.hpp>
#include <core/debug.hpp>
#include <core/string.hpp>
#include <renderer/factory.hpp>

// shader unlit embutido ou carregado de arquivo depois
static const char* VS = R"(#version 330 core
layout(location=0) in vec3 a_pos;
uniform mat4 u_mvp;
void main(){ gl_Position = u_mvp * vec4(a_pos,1.0); }
)";

static const char* FS = R"(#version 330 core
uniform vec4 u_color;
out vec4 frag;
void main(){ frag = u_color; }
)";

static ShaderHandle   g_unlit = {};
static MaterialHandle g_mat_red   = {};
static MaterialHandle g_mat_green = {};
static bool           g_use_green = false_value;

static NodeHandle g_player = {};
static NodeHandle g_pipe   = {};

GLOBAL void _start()
{
    Debug::Println( PrintColorType_Cyan, "Start.." );

    String title;
    title.init( WindowConfig::Get::title() );
    title.append( " - FlappyBirds", true_value );
    WindowConfig::Set::title( title.data() );
    CoreWindow::update_config();

    Scene& scene = Engine::scene();

    g_player = scene.spawn_cube( "Player", 0.f, 0.f, -4.f, 0.5f, 0.5f, 0.5f );
    g_pipe   = scene.spawn_cube( "Pipe",   2.f, 0.f, -4.f, 0.4f, 2.0f, 0.4f );

    scene.spawn_cube_child( g_player, "Hat", 0.f, 0.6f, 0.f, 0.3f, 0.3f, 0.3f );

	g_unlit = Gpu::create_shader( VS, FS, "unlit_color" );

	Material red = {};
    red.color = RedColor;      // { 1, 0, 0, 1 }
	//red.shader = g_unlit;
    g_mat_red = Gpu::create_material( red );

    Material green = {};
    green.color = GreenColor;  // { 0, 1, 0, 1 }
	//green.shader = g_unlit;
    g_mat_green = Gpu::create_material( green );

    // Liga material ao descriptor do node
    Node* player = NodeSystem::get( g_player );
    if ( player )
    {
        Descriptor* d = Gpu::get_descriptor_mutable( player->descriptor );
        if ( d )
        {
            d->handle_material = g_mat_red;
            d->dirty = true_value;
        }
    }
}

GLOBAL void _input()
{
  if ( Keyboard::check_pressed( VK_Space ) )
  {
    g_use_green = !g_use_green;

    Node* player = NodeSystem::get( g_player );
    if ( !player ) return;

    Descriptor* d = Gpu::get_descriptor_mutable( player->descriptor );
    if ( !d ) return;

    d->handle_material = g_use_green ? g_mat_green : g_mat_red;
  }
}

GLOBAL void _update( float_64 delta )
{
    static float_64 t = 0.0;
    t += delta * 0.000002;

    if ( g_player.is_valid() )
    {
		Node* player = NodeSystem::get( g_player );

    	for ( int index = 0; index < player->children.size(); index++  )
    	{
    	     NodeHandle child_handle = player->children.get( index );
    	     Node* child = NodeSystem::get ( child_handle );
    	     const auto t_child = child->local;

    	     float rot_x = t;
    	     float rot_y = t * .5f;
    	     float rot_z = t;

    	     NodeSystem::set_local_rotation( child_handle, rot_x, rot_y, rot_z );
    	}
    }
}
GLOBAL void _draw() {

	// ..
}
GLOBAL void _finish() {
	Gpu::destroy_material( g_mat_red );
    Gpu::destroy_material( g_mat_green );
}