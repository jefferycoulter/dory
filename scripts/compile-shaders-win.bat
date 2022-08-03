@echo off

rem get vulkan directory
cd /./VulkanSDK
for /f "tokens=*" %%i in ('dir /b') do set VULKAN_VERSION=%%i
popd
set VULKAN_DIR="C:\VulkanSDK\%VULKAN_VERSION%"

rem get compiler
set GLSLC="%VULKAN_DIR%\bin\glslc"

rem compile shader programs
pushd %CURRENT_DIR%
cd %CURRENT_DIR%\assets\shaders
for %%i in (*.vert *.frag) do (
    echo "Compiling %%i."
    %GLSLC% %%i -o %%i.spv
    echo "%%i.spv created."
)
popd