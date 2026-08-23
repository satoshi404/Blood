#include <renderer/gpu/command/gpu.command.hpp>

#include <string.h>

void make_label( char *dst, usize size, const char *src )
{
    if (!src)
    {
        dst[0] = '\0';
        return;
    }

    strncpy( dst, src, size - 1 );
    dst[ size - 1 ] = '\0';
}

Command GpuCommand::clear( const Color color, const char *text)
{
    static Command commmand;
    command.type = CommandType_Clear;

    const float r = color.normalized().r;
    const float g = color.normalized().g;
    const float b = color.normalized().b;
    const float a = color.normalized().a;

    command.commands.clear = {  };
    make_label( c.label, sizeof(c.label), text );
    return c;
}

Command Command::draw( DescriptorHandle handle_descriptor, const char *text )
{
    static GpuCommand command;
    command.type = CommandType_Draw;
    command.commands.draw.descriptor = handle_descriptor;
    make_label( command.label, sizeof(command.label), text );
    return command;
}

Command Command::viewport( const i32 viewport_x, const i32 viewport_y, const i32 viewport_w, const i32 viewport_h, const char *text )
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
    static Command comamnd;
    command.type = CommandType_Swap;
    make_label(c.label, sizeof( command.label ), command );
    return command;
}

GpuCommand GpuCommand::transform(const GpuTransform &value, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Set_Transform;
    c.commands.transform = value;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::material(const GpuMaterial *value, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Set_Material;
    c.commands.material.material = value;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::render_state(const GpuRenderState &value, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Set_RenderState;
    c.commands.render_state = value;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::texture(GpuTextureHandle value, u32 slot, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Bind_Texture;
    c.commands.texture.texture = value;
    c.commands.texture.slot = slot;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::push_state(const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Push_State;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::pop_state(const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Pop_State;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::begin_render_pass( const GpuRenderPass& pass, const char* label )
{
    GpuCommand com = {};
    com.type = GpuCommandType_BeginRenderPass;
    com.commands.begin_pass.pass = pass;
    make_label(com.label, sizeof(label)  ,label );
    return com;
}

GpuCommand GpuCommand::end_render_pass( const char* label )
{
    GpuCommand com = {};
    com.type = GpuCommandType_EndRenderPass;
    make_label(com.label, sizeof(label)  ,label );
    return com;
}

GpuCommand GpuCommand::execute_queue( const GpuRenderQueueHandle& queue, const char* label )
{
    GpuCommand com = {};
    com.type = GpuCommandType_EndRenderPass;
    com.commands.execute_pass.queue = queue;
    make_label(com.label, sizeof(label)  ,label );
    return com;
}
