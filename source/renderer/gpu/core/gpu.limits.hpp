#pragma once

#include <core/types.hpp>

namespace GpuLimits
{
    static constexpr u32 MaxDescriptors = 256;
    static constexpr u32 MaxBuffers = 512;
    static constexpr u32 MaxTextures = 512;
    static constexpr u32 MaxShaders = 256;
    static constexpr u32 MaxMeshes = 256;
    static constexpr u32 MaxMaterials = 256;

    static constexpr u32 MaxCommands = 4096;
    static constexpr u32 MaxStateDepth = 32;
    static constexpr u32 MaxPipelinePasses = 32;
    static constexpr u32 MaxLayers = 32;

    static constexpr u32 LabelSize = 64;
}
