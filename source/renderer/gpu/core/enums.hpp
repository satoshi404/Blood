#pragma once

#include <core/types.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////////

enum_type( ShaderType, uint_8 )
{
    ShaderType_Vertex,
    ShaderType_Frag,
};

enum_type( ShaderRuntimeType, uint_8 )
{
    ShaderRuntimeType_DefaultApi,
    ShaderRuntimeType_BloodLanguage,
    ShaderRuntimeType_Abs,
};


enum_type( CommandType, uint_8 )
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

enum_type( DrawType, uint_8 )
{
    DrawType_Cube,
    DrawType_Sphere,
    DrawType_Obj,
    DrawType_Mesh,
    DrawType_Count
};

enum_type( ContextType, uint_8 )
{
    ContextType_2D,
    ContextType_3D,
    ContextType_Count
};

enum_type( TopologiePrimitiveType, uint_8 )
{
    TopologiePrimitiveType_Default = 0,
    TopologiePrimitiveType_Points,
    TopologiePrimitiveType_Lines,
    TopologiePrimitiveType_LineStrip,
    TopologiePrimitiveType_Triangles,
    TopologiePrimitiveType_TriangleStrip,
    TopologiePrimitiveType_TriangleFan
};

enum_type( BufferType, uint_8 )
{
    BufferType_Vertex,
    BufferType_Index,
    BufferType_Uniform,
    BufferType_Constant,
    BufferType_Instance
};

enum_type( BlendModeType, uint_8 )
{
    BlendModeType_None,
    BlendModeType_Alpha,
    BlendModeType_Additive,
    BlendModeType_Multiply
};

enum_type( CullModeType, uint_8 )
{
    CullModeType_Back,
    CullModeType_Front,
    CullModeType_None
};

enum_type( ShaderBindType, uint_8 )
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
