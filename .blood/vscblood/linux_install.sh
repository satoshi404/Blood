#!/bin/sh

VSIX_NAME=vscblood.vsix

mkdir extension
#cp -R syntax       extension/syntax
cp -R extension.js extension
cp -R package.json extension

zip -rqm $VSIX_NAME extension

~/Downloads/vscode/usr/share/code/bin/code --install-extension $VSIX_NAME