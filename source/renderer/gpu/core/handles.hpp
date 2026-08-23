#pragma once

#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DescriptorTag {};
struct BufferTag {};
struct TextureTag {};
struct ShaderTag {};
struct MeshTag {};
struct MaterialTag {};
struct RenderQueueTag {};

using DescriptorHandle   =  Handle<DescriptorTag>;
using BufferHandle       =  Handle<BufferTag>;
using TextureHandle      =  Handle<TextureTag>;
using ShaderHandle       =  Handle<ShaderTag>;
using MeshHandle         =  Handle<MeshTag>;
using MaterialHandle     =  Handle<MaterialTag>;
using RenderQueueHandle  = Handle<RenderQueueTag>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////