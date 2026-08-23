#include <renderer/gpu/resource/descriptor.hpp>
#include <core/math.hpp>
#include <string.h>


DescriptorBuilder& DescriptorBuilder::type( DrawType draw_type )
{
    value.draw_type = draw_type;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::context( ContextType context_type )
{
    value.context_type = context_type;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::mesh( MeshHandle mesh )
{
    value.handle_mesh = mesh;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::material( MaterialHandle material )
{
    value.handle_material = material;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::position( const float_32 x, const float_32 y, const float_32 z)
{
    value.transform.position = fvec3( x, y, z );
    value.transform.mark_dirty();
    value.dirty = true;

    return *this;
}

DescriptorBuilder& DescriptorBuilder::rotation( const float_32 x, const float_32 y, const float_32 z )
{
    value.transform.rotation = fvec3( x, y, z );
    value.transform.mark_dirty();
    value.dirty = true;

    return *this;
}

DescriptorBuilder& DescriptorBuilder::scale( const float_32 x, const float_32 y, const float_32 z )
{
    value.transform.scale = fvec3( x, y, z );
    value.transform.mark_dirty();
    value.dirty = true;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::size( const float_32 w, const float_32 h, const float_32 d )
{
    value.size = { w, h, d };
    value.dirty = true;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::primitive( TopologiePrimitiveType primitive_type )
{
    value.primitive_type = primitive_type;
    value.dirty = true;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::layer( const int_32 layer )
{
    value.layer = layer;
    value.dirty = true;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::z_order( const float_32 z_value_order )
{
    value.z_order = z_value_order;
    value.dirty = true;
    return *this;
}

DescriptorBuilder& DescriptorBuilder::label( const char* label )
{
    if (!label)
    {
        value.label[0] = '\0';
        return *this;
    }

    strncpy(value.label, label, sizeof(value.label) - 1);
    value.label[sizeof(value.label) - 1] = '\0';

    return *this;
}

Descriptor DescriptorBuilder::build() const
{
    return value;
}
