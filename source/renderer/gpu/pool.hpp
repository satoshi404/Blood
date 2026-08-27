#pragma once

#include <renderer/gpu/core/handles.hpp>

#include <renderer/gpu/resource/buffer.hpp>
#include <renderer/gpu/resource/descriptor.hpp>
#include <renderer/gpu/resource/mesh.hpp>
#include <renderer/gpu/resource/texture.hpp>
#include <renderer/gpu/resource/material.hpp>
#include <renderer/gpu/resource/shader.hpp>

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

	// Create
    extern BufferHandle create_buffer( const float_32* data, const uint_32 count );
	extern MeshHandle create_mesh(  const float_32* positions, const uint_32 vertex_count, const uint_32 components = 2 );
	extern DescriptorHandle create_descriptor( const Descriptor& descriptor );
	extern RenderQueueHandle create_queue( const char* label = nullptr );
	extern MaterialHandle create_material( const Material& material );
	extern ShaderHandle create_shader( const Shader& shader );
	extern TextureHandle create_texture( const Texture& texture);

	// Update
	extern bool update_buffer( BufferHandle handle, const Buffer& buffer );
	extern bool update_mesh( MeshHandle handle, const Mesh& buffer );
	extern bool update_descriptor( DescriptorHandle handle, const Descriptor& buffer );
	extern bool update_material( MaterialHandle handle, const Material& material );
	extern bool update_shader( ShaderHandle handle, const Shader& shader );
	extern bool update_texture( TextureHandle handle, const Texture& texture );

	// Destroy
	extern bool destroy_buffer( BufferHandle handle );
	extern bool destroy_mesh( MeshHandle handle );
	extern bool destroy_descriptor( DescriptorHandle handle );
	extern bool destroy_queue( RenderQueueHandle handle );
	extern bool destroy_material( MaterialHandle handle );
	extern bool destroy_shader( ShaderHandle handle );
	extern bool destroy_texture( TextureHandle handle );

	// Get
	extern Buffer* get_buffer( BufferHandle handle );
	extern Mesh* get_mesh( MeshHandle handle );
	extern Descriptor* get_descriptor( DescriptorHandle handle );
	extern RenderQueue* get_queue( RenderQueueHandle handle );
	extern Material* get_material( MaterialHandle handle );
	extern Shader* get_shader( ShaderHandle handle );
	extern Texture* get_texture( TextureHandle handle );

	// Exist
	extern bool exist_buffer( BufferHandle handle );
	extern bool exist_mesh( MeshHandle handle );
	extern bool exist_descriptor( DescriptorHandle handle );
	extern bool exist_queue( RenderQueueHandle handle );
	extern bool exist_material( MaterialHandle handle );
	extern bool exist_shader( ShaderHandle handle );
	extern bool exist_texture( TextureHandle handle );
};