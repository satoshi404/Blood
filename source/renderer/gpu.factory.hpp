#pragma once

// Compatibilidade com o codigo antigo.
// A nova arquitetura fica em renderer/gpu/.
#include <renderer/gpu/gpu.hpp>
#include <renderer/layer/gpu.backend.hpp>

// -----------------------------------------------------------------------------
// Aliases antigos -> novos tipos.
// Assim o restante do engine pode migrar gradualmente.
// -----------------------------------------------------------------------------
using GpuLayer_StatisticsFrame = GpuStatisticsFrame;
using GpuLayer_Statistics      = GpuStatistics;

using GpuLayer_Buffer   = GpuBuffer;
using GpuLayer_Mesh     = GpuMesh;
using GpuLayer_Material = GpuMaterial;

using GpuLayer_DescriptorHandle = GpuDescriptorHandle;
using GpuLayer_Descriptor       = GpuDescriptor;

using GpuLayer_Transform = GpuTransform;
using GpuLayer_Clear = GpuClear;
using GpuLayer_Viewport = GpuViewport;

using GpuLayer_RenderStateCommand = GpuRenderState;
using GpuLayer_Command = GpuCommand;
using GpuLayer_CommandList = GpuCommandList;

using GpuLayer_Draw = GpuDrawCommand;

using CommandType = GpuCommandType;
using DrawCommandType = GpuDrawType;
using Context = GpuContext;
using BufferType = GpuBufferType;
using ShaderBindType = GpuShaderBindType;

// Legacy enum names.
static constexpr auto CommandType_Clear            = GpuCommandType_Clear;
static constexpr auto CommandType_Draw             = GpuCommandType_Draw;
static constexpr auto CommandType_Viewport         = GpuCommandType_Viewport;
static constexpr auto CommandType_Swap              = GpuCommandType_Swap;
static constexpr auto CommandType_Bind_Shader       = GpuCommandType_Bind_Shader;
static constexpr auto CommandType_Bind_Buffer       = GpuCommandType_Bind_Buffer;
static constexpr auto CommandType_UnBind_Buffer     = GpuCommandType_UnBind_Buffer;
static constexpr auto CommandType_Set_Transform     = GpuCommandType_Set_Transform;
static constexpr auto CommandType_Set_Material      = GpuCommandType_Set_Material;
static constexpr auto CommandType_Set_RenderState   = GpuCommandType_Set_RenderState;
static constexpr auto CommandType_Bind_Texture      = GpuCommandType_Bind_Texture;
static constexpr auto CommandType_Push_State        = GpuCommandType_Push_State;
static constexpr auto CommandType_Pop_State         = GpuCommandType_Pop_State;
static constexpr auto CommandType_Count             = GpuCommandType_Count;

static constexpr auto DrawCommandType_Cube   = GpuDrawType_Cube;
static constexpr auto DrawCommandType_Sphere = GpuDrawType_Sphere;
static constexpr auto DrawCommandType_Obj    = GpuDrawType_Obj;
static constexpr auto DrawCommandType_Count  = GpuDrawType_Count;

static constexpr auto Context_2D = GpuContext_2D;
static constexpr auto Context_3D = GpuContext_3D;
static constexpr auto Context_Count = GpuContext_Count;

namespace GpuLayer
{
    inline bool init() { return Gpu::init(); }
    inline void shutdown() { Gpu::shutdown(); }

    inline void new_frame() { Gpu::new_frame(); }
    inline const GpuStatistics& statistics() { return Gpu::statistics(); }

    inline GpuDescriptorHandle create_descriptor(const GpuDescriptor& d)
    {
        return Gpu::create_descriptor(d);
    }

    inline bool update_descriptor(GpuDescriptorHandle h, const GpuDescriptor& d)
    {
        return Gpu::update_descriptor(h, d);
    }

    inline bool destroy_descriptor(GpuDescriptorHandle h)
    {
        return Gpu::destroy_descriptor(h);
    }

    inline const GpuDescriptor* get_descriptor(GpuDescriptorHandle h)
    {
        return Gpu::get_descriptor(h);
    }

    inline GpuCommand make_draw_command(
        GpuDescriptorHandle h,
        const char* label = nullptr)
    {
        return GpuCommand::draw(h, label);
    }

    inline GpuCommand make_clear_command(
        f32 r, f32 g, f32 b, f32 a = 1.0f,
        const char* label = nullptr)
    {
        return GpuCommand::clear(r, g, b, a, label);
    }

    inline GpuCommand make_viewport_command(
        i32 x, i32 y, i32 width, i32 height,
        const char* label = nullptr)
    {
        return GpuCommand::viewport(x, y, width, height, label);
    }

    inline GpuCommand make_swap_command(const char* label = nullptr)
    {
        return GpuCommand::swap(label);
    }

    inline GpuCommand make_transform_command(
        f32 x, f32 y, f32 z,
        const char* label = nullptr)
    {
        GpuTransform t = {};
        t.position[0] = x;
        t.position[1] = y;
        t.position[2] = z;
        return GpuCommand::transform(t, label);
    }

    inline GpuCommand make_material_command(
        const GpuMaterial* material,
        const char* label = nullptr)
    {
        return GpuCommand::material(material, label);
    }

    inline GpuCommand make_render_state_command(
        const GpuRenderState& state,
        const char* label = nullptr)
    {
        return GpuCommand::render_state(state, label);
    }

    inline GpuCommand make_texture_command(
        GpuTextureHandle texture,
        u32 slot,
        const char* label = nullptr)
    {
        return GpuCommand::texture(texture, slot, label);
    }

    inline void bind_command(GpuCommand command)
    {
        Gpu::execute(command);
    }


    inline void submit(GpuCommandList& list)
    {
        Gpu::submit(list);
    }

}
