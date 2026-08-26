#pragma once

#include <renderer/gpu/core/handles.hpp>

#include <renderer/gpu/resource/buffer.hpp>
#include <renderer/gpu/resource/descriptor.hpp>
#include <renderer/gpu/resource/mesh.hpp>
#include <renderer/gpu/resource/texture.hpp>
#include <renderer/gpu/resource/material.hpp>

#include <renderer/gpu/render/queue.hpp>

template <typename T> struct Slot
{
	T value = {};
	uint_32 generation = 0;
	bool alive = false_value;
};

namespace Pool
{
	// TODO: init / free for all pool
    extern bool init();
    extern void free();

	extern signed_size size();
    extern signed_size capacity();

    extern BufferHandle create_buffer( const float_32* data, const uint_32 count );
	extern MeshHandle create_mesh( const Mesh& mesh );
	extern DescriptorHandle create_descriptor( const Descriptor& descriptor );
	extern RenderQueueHandle create_queue( const char* label = nullptr );
	extern MaterialHandle create_material( const Material& material );

	extern bool update_buffer( BufferHandle handle, const Buffer& buffer );
	extern bool update_mesh( MeshHandle handle, const Mesh& buffer );
	extern bool update_descriptor( DescriptorHandle handle, const Descriptor& buffer );
	extern bool update_material( MaterialHandle handle, const Material& material );

	extern bool destroy_buffer( BufferHandle handle );
	extern bool destroy_mesh( MeshHandle handle );
	extern bool destroy_descriptor( DescriptorHandle handle );
	extern bool destroy_queue( RenderQueueHandle handle );
	extern bool destroy_material( MaterialHandle handle );

	extern Buffer* get_buffer( BufferHandle handle );
	extern Mesh* get_mesh( MeshHandle handle );
	extern Descriptor* get_descriptor( DescriptorHandle handle );
	extern RenderQueue* get_queue( RenderQueueHandle handle );
	extern Material* get_material( MaterialHandle handle );

	extern bool exist_buffer( BufferHandle handle );
	extern bool exist_mesh( MeshHandle handle );
	extern bool exist_descriptor( DescriptorHandle handle );
	extern bool exist_queue( RenderQueueHandle handle );
	extern bool exist_material( MaterialHandle handle );
};