#include <renderer/backend/backend.hpp>
#include <renderer/gpu/resource/descriptor.hpp>
#include <core/debug.hpp>

#include <renderer/gpu/pool.hpp>

#if API_OPENGL

namespace
{
    // --- Quad 2D (legado) ---
    Mesh g_cube_quad;
    Material g_quad_material = {};
    bool g_quad_ready = false_value;

    void ensure_quad()
    {
        if (g_quad_ready)
            return;

        static const float_32 vertices[] =
        {
            -50.0f,  50.0f,
             50.0f,  50.0f,
            -50.0f, -50.0f,
             50.0f, -50.0f
        };

        g_quad_material.color = { 0.95f, 0.55f, 0.15f, 1.0f };
        g_cube_quad.primitive = TopologiePrimitiveType_TriangleStrip;

        g_quad_ready = g_cube_quad.upload(vertices, 4, 2);

        if (!g_quad_ready)
            Debug::Println(
                PrintColorType_Red,
                "[Gpu:OpenGL] Falha ao criar quad padrao"
            );
    }

    Mesh g_cube_mesh;
    Material g_cube_material = {};
    bool g_cube_ready = false_value;

    void ensure_cube()
    {
        if (g_cube_ready)
            return;

        static const float_32 vertices[] =
        {
            // Face traseira (Z-)
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            // Face frontal (Z+)
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            // Face esquerda (X-)
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            // Face direita (X+)
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            // Face inferior (Y-)
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            // Face superior (Y+)
            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        g_cube_material.color = ( Color ){ 0.8f, 0.5f , 0. };
        g_cube_mesh.primitive = TopologiePrimitiveType_Triangles;

        // 36 vertices, 3 componentes (x, y, z)
        g_cube_ready = g_cube_mesh.upload(vertices, 36, 3);

        if (!g_cube_ready)
            Debug::Println(
                PrintColorType_Red,
                "[Gpu:OpenGL] Falha ao criar cubo padrao"
            );
    }

    const Mesh& resolve_mesh( const Descriptor& desc, const Mesh& fallback )
    {
        return fallback;
    }

   const Material& resolve_material( const Descriptor& desc, const Material& fallback )
{
    if ( desc.handle_material.is_valid() )
    {
        Material* m = Pool::get_material( desc.handle_material ); // se existir pool
        if ( m ) return *m;
    }
    return fallback;
    }
}

void Backend::draw_cube_2d(const Descriptor& descriptor)
{
   ensure_quad();

   const Mesh& mesh = resolve_mesh( descriptor, g_cube_quad );
   const Material& material = resolve_material( descriptor, g_quad_material );

   material.bind();

   set_transform(descriptor.transform);
   set_render_state(descriptor.render_state);

   TopologiePrimitiveType prim = ( descriptor.primitive_type != TopologiePrimitiveType_Default ) ? descriptor.primitive_type : mesh.primitive;
   mesh_draw( mesh, prim );
}

void Backend::draw_cube_3d(const Descriptor& descriptor)
{
   ensure_cube();

   const Mesh& mesh = resolve_mesh( descriptor, g_cube_mesh );
   const Material& material = resolve_material( descriptor, g_cube_material );

   material.bind();

   set_transform(descriptor.transform);
   set_render_state(descriptor.render_state);

   TopologiePrimitiveType prim = ( descriptor.primitive_type != TopologiePrimitiveType_Default ) ? descriptor.primitive_type : mesh.primitive;
   mesh_draw( mesh, prim );
}

void Backend::draw_sphere_2d(const Descriptor& descriptor )
{
    Debug::Println(
        PrintColorType_Yellow,
        "[Gpu:OpenGL] draw_sphere_2d ainda nao implementado"
    );
}

void Backend::draw_sphere_3d(const Descriptor& descriptor)
{
    Debug::Println(
        PrintColorType_Yellow,
        "[Gpu:OpenGL] draw_sphere_3d ainda nao implementado"
    );
}

void Backend::draw_obj( const Descriptor& descriptor )
{
    Debug::Println(
        PrintColorType_Yellow,
        "[Gpu:OpenGL] draw_obj ainda nao implementado"
    );
}

#endif