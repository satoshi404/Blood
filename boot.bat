@echo off

@rem Platform
clang++ -c source\platform\window.config.cpp -o build\objects\platform.window.config.o -Isource
clang++ -c source\platform\windows\window.cpp -o build\objects\platform.window.windows.o -Isource

@rem Vendor
clang++ -c source\core\debug.cpp -o build\objects\debug.o -Isource

if %errorlevel% neq 0 (
    echo Compilation error fase 1
    pause
    exit /b 1
)
