#pragma once

#include <core/types.hpp>


// TODO: New
enum_type( GpuShaderType, u32 )
{
    GpuShaderType_Vertex,
    GpuShaderType_Frag,
};

enum_type( GpuShaderRuntimeType, u32 )
{
    GpuShaderRuntimeType_DefaultApi,
    GpuShaderRuntimeType_BloodLanguage,
    GpuShaderRuntimeType_Abs,
};


enum_type(GpuCommandType, u32){
    GpuCommandType_Clear,
    GpuCommandType_Draw,
    GpuCommandType_Viewport,
    GpuCommandType_Swap,
    GpuCommandType_Bind_Shader,
    GpuCommandType_Bind_Buffer,
    GpuCommandType_UnBind_Buffer,
    GpuCommandType_Set_Transform,
    GpuCommandType_Set_Material,
    GpuCommandType_Set_RenderState,
    GpuCommandType_Bind_Texture,
    GpuCommandType_Push_State,
    GpuCommandType_Pop_State,
    GpuCommandType_Count};

enum_type(GpuDrawType, u32){
    GpuDrawType_Cube,
    GpuDrawType_Sphere,

    // TODO: New
    GpuDrawType_Obj,
    GpuDrawType_Mesh,

    GpuDrawType_Count};

enum_type(GpuContext, u32){
    GpuContext_2D,
    GpuContext_3D,
    GpuContext_Count};

enum_type( TopologiePrimitive, u32 )
{
    TopologiePrimitive_Default = 0,
    TopologiePrimitive_Points,
    TopologiePrimitive_Lines,
    TopologiePrimitive_LineStrip,
    TopologiePrimitive_Triangles,
    TopologiePrimitive_TriangleStrip,
    TopologiePrimitive_TriangleFan
};

enum_type(GpuBufferType, u8){
    GpuBufferType_Vertex,
    GpuBufferType_Index,
    GpuBufferType_Uniform,
    GpuBufferType_Constant,
    GpuBufferType_Instance};

enum_type(GpuBlendMode, u32){
    GpuBlendMode_None,
    GpuBlendMode_Alpha,
    GpuBlendMode_Additive,
    GpuBlendMode_Multiply};

enum_type(GpuCullMode, u32){
    GpuCullMode_Back,
    GpuCullMode_Front,
    GpuCullMode_None};

enum_type(GpuShaderBindType, u32){
    GpuShaderBindType_Texture2D,
    GpuShaderBindType_Vec2,
    GpuShaderBindType_Vec3,
    GpuShaderBindType_Int,
    GpuShaderBindType_Int32,
    GpuShaderBindType_Int64,
    GpuShaderBindType_Float,
    GpuShaderBindType_Uniform};
