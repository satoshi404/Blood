#pragma once

#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Tag>
struct Handle
{
    static constexpr uint_32 InvalidIndex = UINT_32_MAX;

    uint_32 index = InvalidIndex;
    uint_32 generation = 0;

    bool is_valid() const
    {
        return index != InvalidIndex && generation != 0;
    }

    void reset()
    {
        index = InvalidIndex;
        generation = 0;
    }

    bool operator==( const Handle& other ) const
    {
        return index == other.index && generation == other.generation;
    }

    bool operator!=( const Handle& other ) const
    {
        return !(*this == other);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

struct GpuDescriptorTag {};
struct GpuBufferTag {};
struct GpuTextureTag {};
struct GpuShaderTag {};
struct GpuMeshTag {};
struct GpuMaterialTag {};
struct GpuRenderQueueTag {};

using DescriptorHandle   =  Handle<GpuDescriptorTag>;
using BufferHandle       =  Handle<GpuBufferTag>;
using TextureHandle      =  Handle<GpuTextureTag>;
using ShaderHandle       =  Handle<GpuShaderTag>;
using MeshHandle         =  Handle<GpuMeshTag>;
using MaterialHandle     =  Handle<GpuMaterialTag>;
using RenderQueueHandle  = Handle<GpuRenderQueueTag>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////