#include <core/debug.hpp>
#include <core/string.hpp>
#include <core/types.hpp>
#include <core/math.hpp>

#include <platform/keyboard.hpp>
#include <platform/window.hpp>

#include <engine/engine.hpp>
#include <engine/system/node.hpp>
#include <engine/system/component.hpp>
#include <engine/system/serializer.hpp>

#include <vendor/libc/math.hpp>
#include <string.h>

#include "scripts/bird.hpp"

static NodeHandle g_player_node = {};
static NodeHandle g_camera_node = {};

GLOBAL void _start()
{
	Debug::Println( PrintColorType_Cyan, "Start.." );

	String title;
	title.init( WindowConfig::Get::title() );
	title.append( " -" );
	title.append( "FlappyBirds", true_value );

	WindowConfig::Set::title( title.data() );
	CoreWindow::update_config();

	Scene& scene = Engine::scene();

	// Try load from file first
	const char* scene_path = "runtime/flappy/scenes/main.scene";
	if ( scene.load( scene_path ) )
	{
		Debug::Println( PrintColorType_Green, "Scene loaded from file" );

		// Find nodes by walking children (simple lookup by name)
		Node* root = NodeSystem::get( scene.root );
		if ( root )
		{
			for ( uint_64 i = 0; i < root->children.size(); ++i )
			{
				Node* n = NodeSystem::get( root->children[ i ] );
				if ( !n ) continue;

				if ( strcmp( n->name, "Player" ) == 0 )
					g_player_node = root->children[ i ];
				else if ( strcmp( n->name, "Camera" ) == 0 )
					g_camera_node = root->children[ i ];
			}
		}
	}
	else
	{
		// Fallback: create programmatically
		Debug::Println( PrintColorType_Yellow, "Scene file not found, creating default" );

		g_player_node = scene.create_node( "Player" );
		g_camera_node = scene.create_node( "Camera" );

		if ( g_player_node.is_valid() )
		{
			NodeSystem::set_local_position( g_player_node, 6.f, 0.f, 0.f );
			ComponentSystem::add_render( g_player_node );
			ComponentSystem::add_mesh( g_player_node );
		}

		if ( g_camera_node.is_valid() )
		{
			NodeSystem::set_local_position( g_camera_node, 0.f, 0.f, 5.f );
			CameraComponent* cam = ComponentSystem::add_camera( g_camera_node );
			if ( cam )
			{
				cam->fov = 70.0f;
				cam->near_plane = 0.1f;
				cam->far_plane = 500.0f;
			}
		}

		// Save so next run can load it
		scene.save( scene_path );
	}

	Debug::Println( PrintColorType_Cyan, "Scene → nodes: %llu | components: %llu",
		NodeSystem::count(), ComponentSystem::count() );

	Bird::init();
}


GLOBAL void _input()
{
	if ( Keyboard::check_pressed( VK_Space ) )
	{
		Bird::bump();
		NodeSystem::set_local_scale( g_player_node, 3., 1., 1. );
		NodeSystem::update_world_transforms( g_player_node );
	}

	// F5 = save scene
	if ( Keyboard::check_pressed( VK_F5 ) )
	{
		Engine::scene().save( "runtime/flappy/scenes/main.scene" );
		Debug::Println( PrintColorType_Cyan, "Scene saved (F5)" );
	}
}

GLOBAL void _draw()
{
}

GLOBAL void _update( float_64 delta )
{
	float x = NodeSystem::get( g_player_node )->local.position.x;
	float y = NodeSystem::get( g_player_node )->local.position.y;
	float z = NodeSystem::get( g_player_node )->local.position.z;

	Debug::Println( PrintColorType_Cyan, "Pos= x : %f, y : %f, z : %f", x, y, z );
	if ( g_player_node.is_valid() )
	{
		static float_64 t = 0.0;
		t += delta * 0.001;
		NodeSystem::set_local_position( g_player_node, 0.f, (float_32)(sin(t) * 0.5), 0.f );
	}
}

GLOBAL void _finish()
{
	Debug::Println( PrintColorType_Cyan, "Finish.." );
	g_player_node = {};
	g_camera_node = {};
	Bird::free();
}