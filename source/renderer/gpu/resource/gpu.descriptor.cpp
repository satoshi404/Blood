#include <renderer/gpu/resource/gpu.descriptor.hpp>
#include <string.h>

GpuDescriptorBuilder& GpuDescriptorBuilder::type(GpuDrawType v)
{
    value.type = v;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::context(GpuContext v)
{
    value.context = v;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::mesh(const GpuMesh* v)
{
    value.mesh = v;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::material(const GpuMaterial* v)
{
    value.material = v;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::position(f32 x, f32 y, f32 z)
{
    value.transform.position[0] = x;
    value.transform.position[1] = y;
    value.transform.position[2] = z;
    value.transform.mark_dirty();
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::rotation(f32 x, f32 y, f32 z)
{
    value.transform.rotation[0] = x;
    value.transform.rotation[1] = y;
    value.transform.rotation[2] = z;
    value.transform.mark_dirty();
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::scale(f32 x, f32 y, f32 z)
{
    value.transform.scale[0] = x;
    value.transform.scale[1] = y;
    value.transform.scale[2] = z;
    value.transform.mark_dirty();
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::size(f32 w, f32 h, f32 d)
{
    value.size = { w, h, d };
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::primitive(GpuPrimitive v)
{
    value.primitive = v;
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::layer(i32 v)
{
    value.layer = v;
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::z_order(f32 v)
{
    value.z_order = v;
    value.dirty = true;
    return *this;
}

GpuDescriptorBuilder& GpuDescriptorBuilder::label(const char* v)
{
    if (!v)
    {
        value.label[0] = '\0';
        return *this;
    }

    strncpy(value.label, v, sizeof(value.label) - 1);
    value.label[sizeof(value.label) - 1] = '\0';
    return *this;
}

GpuDescriptor GpuDescriptorBuilder::build() const
{
    return value;
}
