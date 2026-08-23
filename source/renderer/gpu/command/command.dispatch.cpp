#include <renderer/gpu/command/command.dispatch.hpp>
#include <renderer/gpu/pool/descriptor.hpp>
#include <renderer/layer/backend.hpp>
#include <core/debug.hpp>

namespace
{
    typedef void (*DrawFn)(const Descriptor&);

    static DrawFn g_draw_table[ DrawType_Count ][ ContextType_Count ] =
    {
        { Backend::draw_cube_2d,   Backend::draw_cube_3d   },
        { Backend::draw_sphere_2d, Backend::draw_sphere_3d },
        { Backend::draw_obj,    Backend::draw_obj    }
    };

    void execute_draw(const DrawCommand& command)
    {
        const Descriptor* descriptor =
            DescriptorPool::get(command.descriptor);

        if (!descriptor)
        {
            Debug::Println(
                PrintColorType_Red,
                "[Gpu] Draw com descriptor invalido ou expirado"
            );
            return;
        }

        if (descriptor->draw_type >= DrawType_Count ||
            descriptor->context_type >= ContextType_Count)
        {
            Debug::Println(
                PrintColorType_Red,
                "[Gpu] Descriptor '%s' invalido",
                descriptor->label
            );
            return;
        }

        g_draw_table[descriptor->draw_type][descriptor->context_type](*descriptor);
    }
}

void CommandDispatcher::execute(const Command& command)
{
    if (!command.enabled)
        return;

    switch (command.type)
    {
        case CommandType_Clear:
            Backend::clear(command.commands.clear);
            break;

        case CommandType_Draw:
            execute_draw(command.commands.draw);
            break;

        case CommandType_Viewport:
            Backend::viewport(command.commands.viewport);
            break;

        case CommandType_Swap:
            Backend::swap();
            break;

        case CommandType_Set_Transform:
            Backend::set_transform(command.commands.transform);
            break;

        case CommandType_Set_Material:
            if (command.commands.material.material)
                Backend::bind_material( *command.commands.material.material );
            break;

        case CommandType_Set_RenderState:
            Backend::set_render_state(command.commands.render_state);
            break;

        case CommandType_Bind_Texture:
            Backend::bind_texture(command.commands.texture.texture, command.commands.texture.slot);
            break;

        case CommandType_Push_State:
            Backend::push_state();
            break;

        case CommandType_Pop_State:
            Backend::pop_state();
            break;

        case CommandType_BeginRenderPass:
            {
                const RenderPass& pass = command.commands.begin_pass.pass;

                if ( !pass.enabled ) break;

                Backend::viewport( pass.viewport );

                if ( pass.clear_enabled || pass.color.load_op == LoadOpType_Clear )
                {
                    Backend::clear( pass.color.clear );
                }
                // todo:
            }

        break;

        case CommandType_EndRenderPass:
        //..
        break;

        case CommandType_ExecuteRenderPass:
        {
            RenderQueueHandle qh = command.commands.execute_pass.queue;
            RenderQueue* queue = RenderQueuePool::get( qh );
            if ( !queue ) break;

            for ( uint_32 i = 0; i < queue->count; i++ )
            {
                DescriptorHandle dh = queue->items[i];
                if (!dh.is_valid()) continue;

                DrawCommand cmd = {};
                cmd.descriptor = dh;
                execute_draw( cmd );
            }
        }
        break;

        case CommandType_Bind_Shader:
        case CommandType_Bind_Buffer:
        case CommandType_UnBind_Buffer:
            Debug::Println(
                PrintColorType_Yellow,
                "[Gpu] Command %u ainda nao possui implementacao de recurso",
                command.type
            );
            break;

        default:
            Debug::Println(
                PrintColorType_Red,
                "[Gpu] Command invalido: %u",
                command.type
            );
            break;
    }
}

void CommandDispatcher::execute( const Command* commands, uint_32 count )
{
    if (!commands)
        return;

    for (uint_32 i = 0; i < count; ++i)
        execute(commands[i]);
}
