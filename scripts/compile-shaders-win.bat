@echo off

rem get vulkan directory
cd /./VulkanSDK
for /f "tokens=*" %%i in ('dir /b') do set VULKAN_VERSION=%%i
popd
set VULKAN_DIR="C:\VulkanSDK\%VULKAN_VERSION%"

rem get compiler
set GLSLC=%VULKAN_DIR%\bin\glslc

rem compile shader programs
%GLSLC% %CURRENT_DIR%\assets\shaders\shader.vert -o %CURRENT_DIR%\assets\shaders\shader.vert.spv
%GLSLC% %CURRENT_DIR%\assets\shaders\shader.frag -o %CURRENT_DIR%\assets\shaders\shader.frag.spv