#pragma once

#include <core/types.hpp>

#include <pipeline.hpp>
#include <constants.hpp>

enum_type( CommandType, u32 )
{
	CommandType_Clear,
	CommandType_Draw,
	CommandType_Viewport,
	CommandType_Swap,
	CommandType_Bind_Shader,
	CommandType_Bind_Buffer,
	CommandType_UnBind_Buffer,
};

enum_type( ShaderBindType, u32 )
{
	ShaderBindType_2DTexture,
	ShaderBindType_Vec2,
	ShaderBindType_Vec3,
	ShaderBindType_Int,
	ShaderBindType_Int32,
	ShaderBindType_Int64,
	ShaderBindType_Float,
	ShaderBindType_Uniform,
};

enum_type( BufferType, u8 )
{
	BufferType_Vertex,
	BufferType_Fragment,
};

enum_type( DrawCommandType, u32 )
{
	DrawCommandType_Cube,
	DrawCommandType_Sphere,
	DrawCommandType_Obj,
};

struct GpuLayer_StatisticsFrame
{
	u32 drawCalls = 0;
	u32 vertexCount = 0;
	u32 bufferMaps = 0;
	u32 textureBinds = 0;
	u32 shaderBinds = 0;
};

struct GpuLayer_Statistics
{
	GpuLayer_StatisticsFrame frame = { };

	usize gpuMemorySwapchain = 0;
	usize gpuMemoryShaders = 0;
	usize gpuMemoryVertexBuffers = 0;
	usize gpuMemoryInstanceBuffers = 0;
	usize gpuMemoryIndexBuffers = 0;
	usize gpuMemoryUniformBuffers = 0;
	usize gpuMemoryConstantBuffers = 0;
	usize gpuMemoryMutableBuffers = 0;
	usize gpuMemoryTextures = 0;
	usize gpuMemoryRenderTargets = 0;

	usize total_memory() const
	{
		return
			gpuMemorySwapchain +
			gpuMemoryShaders +
			gpuMemoryVertexBuffers +
			gpuMemoryInstanceBuffers +
			gpuMemoryIndexBuffers +
			gpuMemoryUniformBuffers +
			gpuMemoryConstantBuffers +
			gpuMemoryMutableBuffers +
			gpuMemoryTextures +
			gpuMemoryRenderTargets;
	}
};

struct GpuLayer_Descriptor
{
	// ..
};

struct GpuLayer_Material
{
	// ..
};

struct GpuLayer_Frame
{
	// ..
};

struct GpuLayer_Stack
{

}; // ..

struct GpuLayer_Buffer
{
	BufferType type;
	u64 id;
};

struct GpuLayer_Mesh
{
	// ..
};

enum_type( DrawContext, u32 )
{
	DrawContext_2D,
	DrawContext_3D
};

struct GpuLayer_Draw
{
	DrawContext context;
	DrawCommandType type;
};

struct GpuLayer_Command
{
	CommandType type;

	GpuLayer_Draw draw;
};

namespace GpuLayer
{
	extern void bind_command( GpuLayer_Command command_type );
};
