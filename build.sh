#!/bin/bash

set -xe

rm -rf build/objects/*.o

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Core

clang++ -c source/core/debug.cpp -o build/objects/debug.o -Isource

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Platform

## xcb
clang++ -c source/platform/window.config.cpp -o build/objects/window.config.o -Isource
clang++ -c source/platform/xcb/window.cpp -o build/objects/window.xcb.o -Isource
clang++ -c source/platform/keyboard.cpp -o build/objects/keyboard.o -Isource

#///////////////////////////////////////////////////////////////////////////////////////////////////////////
# Renderer

### Opengl

# backend
# USING GLEW / GL YET
# layer
clang++ -c source/renderer/layer/opengl/gpu.opengl.cpp -o build/objects/renderer.layer.opengl.o -Isource
clang++ -c source/renderer/layer/opengl/gpu.opengl.buffer.cpp -o build/objects/renderer.layer.opengl.buffer.o -Isource
clang++ -c source/renderer/layer/opengl/gpu.opengl.draw.cpp -o build/objects/renderer.layer.opengl.draw.o -Isource
clang++ -c source/renderer/layer/opengl/gpu.opengl.material.cpp -o build/objects/renderer.layer.opengl.material.o -Isource
clang++ -c source/renderer/layer/opengl/gpu.opengl.mesh.cpp -o build/objects/renderer.gpu.layer.opengl.mesh.o -Isource
clang++ -c source/renderer/layer/opengl/gpu.opengl.state.cpp -o build/objects/renderer.gpu.layer.opengl.state.o -Isource
clang++ -c source/renderer/layer/opengl/gpu.opengl.texture.cpp -o build/objects/renderer.gpu.layer.opengl.texture.o -Isource

### Naked

# backend
# clang++ -c source/renderer/backend/naked/naked.xcb.cpp -o build/objects/renderer.backend.naked.xcb.o -Isource
# layer
#clang++ -c source/renderer/layer/naked/gpu.naked.cpp -o build/objects/renderer.gpu.layer.naked.o -Isource

# commands
clang++ -c source/renderer/gpu/command/gpu.command.cpp -o build/objects/renderer.gpu.command.o -Isource
clang++ -c source/renderer/gpu/command/gpu.command.dispatch.cpp -o build/objects/renderer.gpu.command.dispatch.o -Isource

# pool
clang++ -c source/renderer/gpu/pool/gpu.buffer.pool.cpp -o build/objects/renderer.gpu.buffer.pool.o -Isource
clang++ -c source/renderer/gpu/pool/gpu.descriptor.pool.cpp -o build/objects/renderer.gpu.descriptor.pool.o -Isource
clang++ -c source/renderer/gpu/pool/gpu.texture.pool.cpp -o build/objects/renderer.gpu.texture.pool.o -Isource

# resource
clang++ -c source/renderer/gpu/resource/gpu.buffer.cpp -o build/objects/renderer.gpu.resource.buffer.o -Isource
clang++ -c source/renderer/gpu/resource/gpu.descriptor.cpp -o build/objects/renderer.resource.descriptor.o -Isource
clang++ -c source/renderer/gpu/resource/gpu.material.cpp -o build/objects/renderer.gpu.resource.material.o -Isource
clang++ -c source/renderer/gpu/resource/gpu.mesh.cpp -o build/objects/renderer.gpu.resource.mesh.o -Isource

# state
clang++ -c source/renderer/gpu/state/gpu.transform.cpp -o build/objects/renderer.gpu.state.transform.o -Isource

## gpu
clang++ -c source/renderer/gpu/gpu.cpp -o build/objects/renderer.gpu.o -Isource

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
