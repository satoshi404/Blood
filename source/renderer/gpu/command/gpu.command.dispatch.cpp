#include <renderer/gpu/command/gpu.command.dispatch.hpp>
#include <renderer/gpu/pool/gpu.descriptor.pool.hpp>
#include <renderer/layer/gpu.backend.hpp>
#include <core/debug.hpp>

namespace
{
    typedef void (*DrawFn)(const GpuDescriptor&);

    static DrawFn g_draw_table[GpuDrawType_Count][GpuContext_Count] =
    {
        { GpuBackend::draw_cube_2d,   GpuBackend::draw_cube_3d   },
        { GpuBackend::draw_sphere_2d, GpuBackend::draw_sphere_3d },
        { GpuBackend::draw_obj,    GpuBackend::draw_obj    }
    };

    void execute_draw(const GpuDrawCommand& command)
    {
        const GpuDescriptor* descriptor =
            GpuDescriptorPool::get(command.descriptor);

        if (!descriptor)
        {
            Debug::Println(
                PrintColor_Red,
                "[Gpu] Draw com descriptor invalido ou expirado"
            );
            return;
        }

        if (descriptor->type >= GpuDrawType_Count ||
            descriptor->context >= GpuContext_Count)
        {
            Debug::Println(
                PrintColor_Red,
                "[Gpu] Descriptor '%s' invalido",
                descriptor->label
            );
            return;
        }

        g_draw_table[descriptor->type][descriptor->context](*descriptor);
    }
}

void GpuCommandDispatcher::execute(const GpuCommand& command)
{
    if (!command.enabled)
        return;

    switch (command.type)
    {
        case GpuCommandType_Clear:
            GpuBackend::clear(command.commands.clear);
            break;

        case GpuCommandType_Draw:
            execute_draw(command.commands.draw);
            break;

        case GpuCommandType_Viewport:
            GpuBackend::viewport(command.commands.viewport);
            break;

        case GpuCommandType_Swap:
            GpuBackend::swap();
            break;

        case GpuCommandType_Set_Transform:
            GpuBackend::set_transform(command.commands.transform);
            break;

        case GpuCommandType_Set_Material:
            if (command.commands.material.material)
                GpuBackend::material_bind(*command.commands.material.material);
            break;

        case GpuCommandType_Set_RenderState:
            GpuBackend::set_render_state(command.commands.render_state);
            break;

        case GpuCommandType_Bind_Texture:
            GpuBackend::bind_texture(command.commands.texture.texture, command.commands.texture.slot);
            break;

        case GpuCommandType_Push_State:
            GpuBackend::push_state();
            break;

        case GpuCommandType_Pop_State:
            GpuBackend::pop_state();
            break;

        case GpuCommandType_Bind_Shader:
        case GpuCommandType_Bind_Buffer:
        case GpuCommandType_UnBind_Buffer:
            Debug::Println(
                PrintColor_Yellow,
                "[Gpu] Command %u ainda nao possui implementacao de recurso",
                command.type
            );
            break;

        default:
            Debug::Println(
                PrintColor_Red,
                "[Gpu] Command invalido: %u",
                command.type
            );
            break;
    }
}

void GpuCommandDispatcher::execute(const GpuCommand* commands, u32 count)
{
    if (!commands)
        return;

    for (u32 i = 0; i < count; ++i)
        execute(commands[i]);
}
