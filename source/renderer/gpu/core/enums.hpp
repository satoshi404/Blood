#pragma once

#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

enum_type( ShaderType, u8 )
{
    ShaderType_Vertex,
    ShaderType_Frag,
};

enum_type( ShaderRuntimeType, u8 )
{
    ShaderRuntimeType_DefaultApi,
    ShaderRuntimeType_BloodLanguage,
    ShaderRuntimeType_Abs,
};


enum_type( CommandType, u8 )
{
    CommandType_Clear,
    CommandType_Draw,
    CommandType_Viewport,
    CommandType_Swap,
    CommandType_Bind_Shader,
    CommandType_Bind_Buffer,
    CommandType_UnBind_Buffer,
    CommandType_Set_Transform,
    CommandType_Set_Material,
    CommandType_Set_RenderState,
    CommandType_Bind_Texture,
    CommandType_Push_State,
    CommandType_Pop_State,
    CommandType_BeginRenderPass,
    CommandType_EndRenderPass,
    CommandType_ExecuteRenderPass,
    CommandType_Count
};

enum_type( DrawType, u8 )
{
    DrawType_Cube,
    DrawType_Sphere,
    DrawType_Obj,
    DrawType_Mesh,
    DrawType_Count
};

enum_type( Context, u8 )
{
    Context_2D,
    Context_3D,
    Context_Count
};

enum_type( TopologiePrimitiveType, u8 )
{
    TopologiePrimitiveType_Default = 0,
    TopologiePrimitiveType_Points,
    TopologiePrimitiveType_Lines,
    TopologiePrimitiveType_LineStrip,
    TopologiePrimitiveType_Triangles,
    TopologiePrimitiveType_TriangleStrip,
    TopologiePrimitiveType_TriangleFan
};

enum_type( BufferType, u8 )
{
    BufferType_Vertex,
    BufferType_Index,
    BufferType_Uniform,
    BufferType_Constant,
    BufferType_Instance
};

enum_type( BlendModeType, u8 )
{
    BlendModeType_None,
    BlendModeType_Alpha,
    BlendModeType_Additive,
    BlendModeType_Multiply
};

enum_type( CullModeType, u8 )
{
    CullModeType_Back,
    CullModeType_Front,
    CullModeType_None
};

enum_type( ShaderBindType, u8 )
{
    ShaderBindType_Texture2D,
    ShaderBindType_Vec2,
    ShaderBindType_Vec3,
    ShaderBindType_Int,
    ShaderBindType_Int32,
    ShaderBindType_Int64,
    ShaderBindType_Float,
    ShaderBindType_Uniform
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
