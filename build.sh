#!/bin/bash

set -xe

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Core

clang++ -c source/core/debug.cpp -o build/objects/debug.o -Isource

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Platform

## xcb
clang++ -c source/platform/window.config.cpp -o build/objects/window.config.o -Isource
clang++ -c source/platform/xcb/window.cpp -o build/objects/window.xcb.o -Isource

## keyboard
clang++ -c source/platform/keyboard.cpp -o build/objects/keyboard.o -Isource

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Renderer

## gpu layer
clang++ -c source/renderer/gpu.cpp -o build/objects/renderer.gpu.o -Isource

## naked api
clang++ -c source/renderer/naked/naked.xcb.cpp -o build/objects/renderer.naked.xcb.o -Isource

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Vendor

## x11
clang++ -c source/vendor/x11/xutil.cpp -o build/objects/x11_xutil.o -Isource

#///////////////////////////////////////////////////////////////////////////////////////////////////////////

./runtime/build.sh

# Checks if the user forgot to type "-run"
if [ "$1" == "-run" ]; then
    ./build/blood
    exit 1
fi
