#include <renderer/layer/gpu.backend.hpp>
#include <core/debug.hpp>

#if API_OPENGL

namespace
{
    // --- Quad 2D (legado) ---
    GpuMesh g_cube_quad;
    GpuMaterial g_quad_material = {};
    bool g_quad_ready = false_value;

    void ensure_quad()
    {
        if (g_quad_ready)
            return;

        static const f32 vertices[] =
        {
            -50.0f,  50.0f,
             50.0f,  50.0f,
            -50.0f, -50.0f,
             50.0f, -50.0f
        };

        g_quad_material.color = { 0.95f, 0.55f, 0.15f, 1.0f };
        g_cube_quad.primitive = TopologiePrimitive_TriangleStrip;

        g_quad_ready = g_cube_quad.upload(vertices, 4, 2);

        if (!g_quad_ready)
            Debug::Println(
                PrintColor_Red,
                "[Gpu:OpenGL] Falha ao criar quad padrao"
            );
    }

    // --- Cubo 3D (novo) ---
    GpuMesh g_cube_mesh;
    GpuMaterial g_cube_material = {};
    bool g_cube_ready = false_value;

    void ensure_cube()
    {
        if (g_cube_ready)
            return;

        static const f32 vertices[] =
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

        g_cube_material.color = { 0.95f, 0.55f, 0.15f, 1.0f };
        g_cube_mesh.primitive = TopologiePrimitive_Triangles;

        // 36 vertices, 3 componentes (x, y, z)
        g_cube_ready = g_cube_mesh.upload(vertices, 36, 3);

        if (!g_cube_ready)
            Debug::Println(
                PrintColor_Red,
                "[Gpu:OpenGL] Falha ao criar cubo padrao"
            );
    }

    const GpuMesh& resolve_mesh( const GpuDescriptor& desc, const GpuMesh& fallback )
    {
        return fallback;
    }

    const GpuMaterial& resolve_material( const GpuDescriptor& desc, const GpuMaterial& fallback )
    {
        return fallback;
    }
}

void GpuBackend::draw_cube_2d(const GpuDescriptor& descriptor)
{
   ensure_quad();

   const GpuMesh& mesh = resolve_mesh( descriptor, g_cube_quad );
   const GpuMaterial& material = resolve_material( descriptor, g_quad_material );

   material.bind();

   set_transform(descriptor.transform);
   set_render_state(descriptor.render_state);

   TopologiePrimitive prim = ( descriptor.primitive != TopologiePrimitive_Default ) ? descriptor.primitive : mesh.primitive;
   mesh_draw( mesh, prim );
}

void GpuBackend::draw_cube_3d(const GpuDescriptor& descriptor)
{
   ensure_cube();

   const GpuMesh& mesh = resolve_mesh( descriptor, g_cube_mesh );
   const GpuMaterial& material = resolve_material( descriptor, g_cube_material );

   material.bind();

   set_transform(descriptor.transform);
   set_render_state(descriptor.render_state);

   TopologiePrimitive prim = ( descriptor.primitive != TopologiePrimitive_Default ) ? descriptor.primitive : mesh.primitive;
   mesh_draw( mesh, prim );
}

void GpuBackend::draw_sphere_2d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:OpenGL] draw_sphere_2d ainda nao implementado"
    );
}

void GpuBackend::draw_sphere_3d(const GpuDescriptor&)
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:OpenGL] draw_sphere_3d ainda nao implementado"
    );
}

void GpuBackend::draw_obj( const GpuDescriptor &desc )
{
    Debug::Println(
        PrintColor_Yellow,
        "[Gpu:OpenGL] draw_obj ainda nao implementado"
    );
}

#endif