#!/bin/bash
set -xe

mkdir -p build/objects
rm -rf build/objects/*.o

clang++ -c source/core/debug.cpp -o build/objects/debug.o -Isource
clang++ -c source/platform/keyboard.cpp -o build/objects/keyboard.o -Isource
clang++ -c source/platform/window.config.cpp -o build/objects/window.config.o -Isource
clang++ -c source/renderer/gpu.factory.cpp -o build/objects/gpu.factory.o -Isource
clang++ -c source/renderer/gpu.opengl.cpp -o build/objects/gpu.opengl.opengl.o -Isource
clang++ -c source/platform/xcb/window.cpp -o build/objects/window.xcb.o -Isource
clang++ -c source/vendor/x11/xutil.cpp -o build/objects/xutil.o -Isource
# Fase de Linkagem Linux
clang++ build/objects/*.o -o build/blood -lX11 -lxcb -lGL

./runtime/build.sh

if [ "$1" == "-run" ]; then
    ./build/blood
    exit 1
fi
