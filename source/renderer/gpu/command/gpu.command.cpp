#include <renderer/gpu/command/gpu.command.hpp>

#include <string.h>

void make_label( char *dst, unsigned_size size, const char *src )
{
    if (!src)
    {
        dst[0] = '\0';
        return;
    }

    strncpy( dst, src, size - 1 );
    dst[ size - 1 ] = '\0';
}

Command Command::clear( Color color, const char *text)
{
    static Command command;
    command.type = CommandType_Clear;

    const float r = color.normalized().r;
    const float g = color.normalized().g;
    const float b = color.normalized().b;
    const float a = color.normalized().a;

    command.commands.clear = { r, g, b, a };
    make_label( command.label, sizeof(command.label), text );
    return command;
}

Command Command::draw( DescriptorHandle handle_descriptor, const char *text )
{
    static Command command;
    command.type = CommandType_Draw;
    command.commands.draw.descriptor = handle_descriptor;
    make_label( command.label, sizeof(command.label), text );
    return command;
}

Command Command::viewport( const int_32 viewport_x, const int_32 viewport_y, const int_32 viewport_w, const int_32 viewport_h, const char *text )
{
    static Command command;
    command.type = CommandType_Viewport;

    command.commands.viewport =
    {
        viewport_x,
        viewport_y,
        viewport_w,
        viewport_h
    };
    make_label( command.label, sizeof(command.label), text );
    return command;
}

Command Command::swap( const char *text )
{
    Command command;
    command.type = CommandType_Swap;
    make_label( command.label, sizeof( command.label ), text );
    return command;
}

Command Command::transform(const Transform &value, const char *text)
{
    Command command;
    command.type = CommandType_Set_Transform;
    command.commands.transform = value;
    make_label(command.label, sizeof(command.label), text);
    return command;
}

Command Command::material( const Material *value, const char *text)
{
    Command command;
    command.type = CommandType_Set_Material;
    command.commands.material.material = value;
    make_label(command.label, sizeof(command.label), text);
    return command;
}

Command Command::render_state(const RenderState &value, const char *text)
{
    Command command;
    command.type = CommandType_Set_RenderState;
    command.commands.render_state = value;
    make_label(command.label, sizeof(command.label), text);
    return command;
}

Command Command::texture( TextureHandle value, uint_32 slot, const char *text )
{
    Command command = {};
    command.type = CommandType_Bind_Texture;
    command.commands.texture.texture = value;
    command.commands.texture.slot = slot;
    make_label(command.label, sizeof(command.label), text);
    return command;
}

Command Command::push_state(const char *text)
{
    Command command;
    command.type = CommandType_Push_State;
    make_label(command.label, sizeof(command.label), text);
    return command;
}

Command Command::pop_state(const char *text)
{
    Command command;
    command.type = CommandType_Pop_State;
    make_label(command.label, sizeof(command.label), text);
    return command;
}

Command Command::begin_render_pass( const RenderPass& pass, const char* label )
{
    static Command command;
    command.type = CommandType_BeginRenderPass;
    command.commands.begin_pass.pass = pass;
    make_label( command.label, sizeof(command.label)  ,label );
    return command;
}

Command Command::end_render_pass( const char* label )
{
    static Command command;
    command.type = CommandType_EndRenderPass;
    make_label(command.label, sizeof(command.label)  ,label );
    return command;
}

Command Command::execute_queue( const RenderQueueHandle& queue, const char* label )
{
    static Command command;
    command.type = CommandType_EndRenderPass;
    command.commands.execute_pass.queue = queue;
    make_label( command.label, sizeof(command.label)  ,label );
    return command;
}
