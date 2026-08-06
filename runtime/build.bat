@echo off

clang++.exe -c code\main.cpp -o ..\build\main.o -I..\source

clang++.exe ..\build\objects\debug.o ..\build\main.o  ..\build\objects\platform.window.windows.o ..\build\objects\platform.window.config.o -o ..\build\blood.exe -ld3d12 -ld3dcompiler -ldxgi -static-libgcc -luser32 -lkernel32  -I..\source

if %errorlevel% neq 0 (
    echo Compilation error fase 2
    pause
    exit /b 1
)

..\build\blood.exe
