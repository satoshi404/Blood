#pragma once

#include <core/types.hpp>

template<typename Tag>
struct Handle
{
    static constexpr u32 InvalidIndex = 0xFFFFFFFFu;

    u32 index = InvalidIndex;
    u32 generation = 0;

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

struct GpuDescriptorTag {};
struct GpuBufferTag {};
struct GpuTextureTag {};
struct GpuShaderTag {};
struct GpuMeshTag {};
struct GpuMaterialTag {};
struct GpuRenderQueueTag {};

using GpuDescriptorHandle =  Handle<GpuDescriptorTag>;
using GpuBufferHandle     =  Handle<GpuBufferTag>;
using GpuTextureHandle    =  Handle<GpuTextureTag>;
using GpuShaderHandle     =  Handle<GpuShaderTag>;
using GpuMeshHandle       =  Handle<GpuMeshTag>;
using GpuMaterialHandle   =  Handle<GpuMaterialTag>;
using GpuRenderQueueHandle = Handle<GpuRenderQueueTag>;
