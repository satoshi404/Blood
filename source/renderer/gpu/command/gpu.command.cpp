#include <renderer/gpu/command/gpu.command.hpp>
#include <string.h>

void make_label(char *dst, usize size, const char *src)
{
    if (!src)
    {
        dst[0] = '\0';
        return;
    }
    strncpy(dst, src, size - 1);
    dst[size - 1] = '\0';
}

GpuCommand GpuCommand::clear(f32 r, f32 g, f32 b, f32 a, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Clear;
    c.commands.clear = {r, g, b, a};
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::draw(GpuDescriptorHandle descriptor, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Draw;
    c.commands.draw.descriptor = descriptor;
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::viewport(i32 x, i32 y, i32 w, i32 h, const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Viewport;
    c.commands.viewport = {x, y, w, h};
    make_label(c.label, sizeof(c.label), text);
    return c;
}

GpuCommand GpuCommand::swap(const char *text)
{
    GpuCommand c = {};
    c.type = GpuCommandType_Swap;
    make_label(c.label, sizeof(c.label), text);
    return c;
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
