#pragma once

#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Limits
{
    static constexpr u32 Max_Descriptors = 256;
    static constexpr u32 Max_Buffers = 512;
    static constexpr u32 Max_Textures = 512;
    static constexpr u32 Max_Shaders = 256;
    static constexpr u32 Max_Meshes = 256;
    static constexpr u32 Max_Materials = 256;

    static constexpr u32 Max_Commands = 4096;
    static constexpr u32 Max_StateDepth = 32;
    static constexpr u32 Max_PipelinePasses = 32;
    static constexpr u32 Max_Layers = 32;

    static constexpr u32 Label_Size = 64;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////