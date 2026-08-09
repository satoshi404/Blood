#pragma once

#include <core/types.hpp>

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

enum_type(GpuPrimitive, u32){
    GpuPrimitive_Default = 0,
    GpuPrimitive_Points,
    GpuPrimitive_Lines,
    GpuPrimitive_LineStrip,
    GpuPrimitive_Triangles,
    GpuPrimitive_TriangleStrip,
    GpuPrimitive_TriangleFan};

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
