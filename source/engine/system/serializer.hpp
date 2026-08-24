#pragma once

#include <core/types.hpp>
#include <engine/system/node.hpp>
#include <core/string.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Scene Serialization
// Text format (.scene) - human readable, easy to diff and edit by hand.
//
// Example:
//
// scene "MainScene"
// {
//     version = 1
//
//     node "Player"
//     {
//         active = true
//         visible = true
//         position = 0.0 1.0 0.0
//         rotation = 0.0 0.0 0.0
//         scale = 1.0 1.0 1.0
//
//         component Render
//         {
//             layer = 0
//             z_order = 1.0
//         }
//
//         component Camera
//         {
//             fov = 70.0
//             near = 0.1
//             far = 500.0
//         }
//
//         node "Weapon"
//         {
//             position = 0.5 0.0 0.0
//         }
//     }
// }

namespace SceneSerializer
{
    // Save current scene hierarchy to a .scene file
    bool save( const Scene& scene, const char* path );

    // Load a .scene file into an already initialized Scene
    // (clears existing nodes under root first)
    bool load( Scene& scene, const char* path );

    // Helpers
    bool save_node_recursive( NodeHandle node, String& out, int_32 indent );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Extend Scene with serialization

// (declared here so Scene users can call scene.save / scene.load)
// Actual methods are added to the Scene struct in node.hpp

///////////////////////////////////////////////////////////////////////////////////////////////////////////