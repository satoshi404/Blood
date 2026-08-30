@echo off

mkdir extension
xcopy extension.js extension
xcopy package.json extension

7z a -tzip -sdel vscblood.vsix extension
code --install-extension vscblood.vsix

pause