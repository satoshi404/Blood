#include <pipeline.hpp>
#include <constants.hpp>
#include <core/string.hpp>
#include <core/types.hpp>

#define MAX_PATH_CHILDREN ( U8_MAX )

struct BuildGenerator
{
    char* path;
    char* absolute_path;
    u32 index_path;
    char* children[ MAX_PATH_CHILDREN ];
    u32 index_children;
    u32 count_children;
};

static bool string_contains(const char* str, const char* find)
{
    if (!str || !find) return false;
    size_t str_len = strlen(str);
    size_t find_len = strlen(find);

    if (find_len > str_len) return false;

    for (size_t i = 0; i <= str_len - find_len; i++)
    {
        if (strncmp(&str[i], find, find_len) == 0)
        {
            return true;
        }
    }
    return false;
}

static void fix_path_slashes(char* path)
{
    if (!path) return;
    for (size_t i = 0; path[i] != '\0'; i++)
    {
        #if PLATFORM_WINDOWS
            if (path[i] == '/') path[i] = '\\';
        #elif PLATFORM_LINUX
            if (path[i] == '\\') path[i] = '/';
        #endif
    }
}

static String get_object_filename(const char* filepath)
{
    const char* filename = filepath;
    for (size_t i = 0; filepath[i] != '\0'; i++)
    {
        if (filepath[i] == '/' || filepath[i] == '\\')
        {
            filename = &filepath[i + 1];
        }
    }

    char obj_name[256] = {0};
    strncpy(obj_name, filename, sizeof(obj_name) - 1);

    size_t len = strlen(obj_name);
    if (len > 4 && strcmp(&obj_name[len - 4], ".cpp") == 0)
    {
        obj_name[len - 4] = '\0';
    }

    if (string_contains(filepath, "xcb")) strcat(obj_name, ".xcb");
    if (string_contains(filepath, "windows")) strcat(obj_name, ".windows");
    if (string_contains(filepath, "d3d12")) strcat(obj_name, ".d3d12");
    if (string_contains(filepath, "opengl")) strcat(obj_name, ".opengl");
    if (string_contains(filepath, "vulkan")) strcat(obj_name, ".vulkan");

    strcat(obj_name, ".o");
    return String(obj_name);
}

static String build_compile_command(const char* filepath)
{
    String cmd;
    cmd.append("clang++ -c ");
    cmd.append(filepath);
    cmd.append(" -o build/objects/");

    String obj = get_object_filename(filepath);
    cmd.append(obj.data());

    cmd.append(" -Isource\n");
    return cmd;
}

static String generate_platform_build(const BuildGenerator& root)
{
    String script;

    // 1. Cabeçalho do Script
    #if PLATFORM_WINDOWS
        script.append("@echo off\n\n");
        script.append("@rem Removendo objetos antigos\n");
        script.append("if not exist build\\objects mkdir build\\objects\n");
        script.append("del /q build\\objects\\*.o 2>nul\n\n");
    #elif PLATFORM_LINUX
        script.append("#!/bin/bash\nset -xe\n\n");
        script.append("mkdir -p build/objects\n");
        script.append("rm -rf build/objects/*.o\n\n");
    #endif

    // 2. Comandos de Compilação
    for (u32 i = 0; i < root.count_children; i++)
    {
        const char* child = root.children[i];
        if (!child) continue;

        #if PLATFORM_LINUX
            if (string_contains(child, "windows") || string_contains(child, "d3d12") || string_contains(child, "dxgi")) continue;
        #elif PLATFORM_WINDOWS
            if (string_contains(child, "xcb") || string_contains(child, "x11") || string_contains(child, "xutil")) continue;
        #endif

        String cmd = build_compile_command(child);
        script.append(cmd);
    }

    // 3. Verificação de Erro pós-compilação (Windows)
    #if PLATFORM_WINDOWS
        script.append("\nif %errorlevel% neq 0 (\n");
        script.append("    echo Compilation error fase 1\n");
        script.append("    pause\n");
        script.append("    exit /b 1\n");
        script.append(")\n\n");
    #endif

    // 4. Comando de Linkagem Final Dinâmico
    #if PLATFORM_WINDOWS
        script.append("@rem Fase de Linkagem Windows\n");
        script.append("clang++ build\\objects\\*.o -o build\\blood.exe -luser32 -lgdi32 ");
        #if API_D3D12
            script.append("-ld3d12 -ldxgi\n");
        #elif API_OPENGL
            script.append("-lopengl32\n");
        #elif API_VULKAN
            script.append("-lvulkan-1\n");
        #else
            script.append("\n");
        #endif
    #elif PLATFORM_LINUX
        script.append("# Fase de Linkagem Linux\n");\
        // TODO:
        // -lX11 -lX11-xcb -lxcb -lxcb-keysyms -lGL -lGLEW  -lm -lc
        //
        script.append("clang++ build/objects/*.o -o build/blood -lX11 -lxcb ");
        #if API_OPENGL
            script.append("-lGL\n");
        #elif API_VULKAN
            script.append("-lvulkan\n");
        #else
            script.append("\n");
        #endif
    #endif

    // 5. Rodapé de execução
    #if PLATFORM_WINDOWS
        script.append("\nif %errorlevel% neq 0 (\n");
        script.append("    echo Linkage error fase 2\n");
        script.append("    pause\n");
        script.append("    exit /b 1\n");
        script.append(")\n");
    #elif PLATFORM_LINUX
        script.append("\n./runtime/build.sh\n\n");
        script.append("if [ \"$1\" == \"-run\" ]; then\n");
        script.append("    ./build/blood\n");
        script.append("    exit 1\n");
        script.append("fi\n");
    #endif

    return script;
}

static bool save_build_script(const String& content)
{
    const char* filename = nullptr;
    #if PLATFORM_LINUX
        filename = "generate_build.sh";
    #elif PLATFORM_WINDOWS
        filename = "generate_build.bat";
    #endif

    return content.save(filename);
}

int main()
{
    BuildGenerator root = {0};
    root.path = "source/";

    u32 index = 0;

    // Todos os caminhos usam index++ de forma consistente desde o início
    root.children[ index++ ] = "source/core/debug.cpp";
    root.children[ index++ ] = "source/platform/keyboard.cpp";
    root.children[ index++ ] = "source/platform/window.config.cpp";
    root.children[ index++ ] = "source/renderer/gpu.factory.cpp";

    // Seleção de API Gráfica via Macros
    #if API_D3D12
        #if PLATFORM_WINDOWS
            root.children[ index++ ] = "source/renderer/gpu.d3d12.cpp";
        #else
            #error "DirectX12 just works on Windows platform"
        #endif
    #elif API_OPENGL
        root.children[ index++ ] = "source/renderer/gpu.opengl.cpp";
    #elif API_VULKAN
        root.children[ index++ ] = "source/renderer/gpu.vulkan.cpp";
    #elif API_NAKED
        root.children[ index++ ] = "source/renderer/gpu.naked.cpp";
    #endif

    // Seleção de Plataforma de Janela
    #if PLATFORM_WINDOWS
        root.children[ index++ ] = "source/platform/windows/window.cpp";
    #else
        root.children[ index++ ] = "source/platform/xcb/window.cpp";
        root.children[ index++ ] = "source/vendor/x11/xutil.cpp";
    #endif

    root.count_children = index;

    for(u32 i = 0; i < root.count_children; i++) {
        fix_path_slashes(root.children[i]);
    }

    String final_script = generate_platform_build(root);

    if (!save_build_script(final_script))
    {
        return exit_failed_code;
    }

    return exit_success_code;
}
