#include <renderer/gpu/resource/mesh.hpp>
#include <renderer/backend/backend.hpp>

bool Mesh::upload( const float_32* positions, uint_32 count, uint_32 comps )
{
    if (!positions || count == 0 || comps == 0)
        return false_value;

    components = comps;
    vertex_count = count;

    if (!vertex_buffer.create(positions, count * comps))
    {
        vertex_count = 0;
        return false_value;
    }

    return true_value;
}

void Mesh::draw( TopologiePrimitiveType override ) const
{
    TopologiePrimitiveType primitive;
    if ( override != TopologiePrimitiveType_Default ) primitive = override;
    else primitive = this->primitive;
    Backend::mesh_draw( *this, primitive );
}

void Mesh::destroy()
{
    vertex_buffer.destroy();
    vertex_count = 0;
}
