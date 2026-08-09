#include <renderer/layer/gpu.backend.hpp>
#include <core/debug.hpp>

#if API_NAKED

#include <renderer/backend/naked/naked.hpp>

static u8 clamp_color(f32 value)
{
    if (value <= 0.0f) return 0;
    if (value >= 1.0f) return 255;
    return static_cast<u8>(value * 255.0f);
}

bool GpuBackend::init()
{
    return true_value;
}

void GpuBackend::shutdown()
{
}

void GpuBackend::clear(const GpuClear& c)
{
    Naked::clear_color(
        clamp_color(c.r),
        clamp_color(c.g),
        clamp_color(c.b)
    );
}

void GpuBackend::viewport(const GpuViewport&)
{
}

void GpuBackend::swap()
{
    Naked::swap();
}

bool GpuBackend::buffer_create(GpuBuffer& buffer, const f32*, u32)
{
    buffer.native_id = 0;
    return false_value;
}

void GpuBackend::buffer_destroy(GpuBuffer&)
{
}

void GpuBackend::mesh_draw(const GpuMesh&, GpuPrimitive)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:Naked] mesh_draw nao se aplica"
    );
}

void GpuBackend::material_bind(const GpuMaterial& material)
{
    Naked::set_draw_color(
        clamp_color(material.color.r),
        clamp_color(material.color.g),
        clamp_color(material.color.b)
    );
}

void GpuBackend::set_transform(const GpuTransform&)
{
}

void GpuBackend::set_render_state(const GpuRenderState&)
{
}

void GpuBackend::bind_shader(GpuShaderHandle)
{
}

void GpuBackend::bind_texture(GpuTextureHandle, u32)
{
}

void GpuBackend::push_state()
{
}

void GpuBackend::pop_state()
{
}

void GpuBackend::draw_cube_2d(const GpuDescriptor& desc)
{
    if (desc.material)
        material_bind(*desc.material);
    else
        Naked::set_draw_color(255, 255, 255);

    Naked_Rect rect;
    rect.x = static_cast<i32>(desc.transform.position[0]);
    rect.y = static_cast<i32>(desc.transform.position[1]);
    rect.w = static_cast<i32>(desc.size.width);
    rect.h = static_cast<i32>(desc.size.height);

    Naked::draw_rectangle(rect);
}

void GpuBackend::draw_cube_3d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:Naked] 3D nao suportado"
    );
}

void GpuBackend::draw_sphere_2d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:Naked] sphere_2d ainda nao implementado"
    );
}

void GpuBackend::draw_sphere_3d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:Naked] 3D nao suportado"
    );
}

void GpuBackend::draw_obj_2d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:Naked] obj_2d ainda nao implementado"
    );
}

void GpuBackend::draw_obj_3d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:Naked] 3D nao suportado"
    );
}

#endif
