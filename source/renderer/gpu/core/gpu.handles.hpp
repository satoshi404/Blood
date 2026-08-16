#pragma once

#include <core/types.hpp>

template<typename Tag>
struct GpuHandle
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

    bool operator==( const GpuHandle& other ) const
    {
        return index == other.index && generation == other.generation;
    }

    bool operator!=( const GpuHandle& other ) const
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

using GpuDescriptorHandle = GpuHandle<GpuDescriptorTag>;
using GpuBufferHandle     = GpuHandle<GpuBufferTag>;
using GpuTextureHandle    = GpuHandle<GpuTextureTag>;
using GpuShaderHandle     = GpuHandle<GpuShaderTag>;
using GpuMeshHandle       = GpuHandle<GpuMeshTag>;
using GpuMaterialHandle   = GpuHandle<GpuMaterialTag>;
