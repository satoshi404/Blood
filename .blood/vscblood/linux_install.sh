#!/bin /bash

mkdir extension
cp -R extension.js extension
cp -R package.json extension

zip -rqm vscblood.vsix extension
codebin --install-extension vscblood.vsix

rem -rf extension