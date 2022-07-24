@echo off

rem get current directory
set CURRENT_DIR=%cd%

rem save the current directory in memory
pushd %CURRENT_DIR%
rem look for cmake in Program Files
cd /./Program Files
for /f "tokens=*" %%i in ('dir LLVM') do set LLVM_RESULT=%%i
rem return to CURRENT_DIR
popd
rem if cmake wasn't found, then install it
if "%LLVM%" == "Directory of C:\Program Files" (
    echo "Installing LLVM..."
    winget install -e --id Kitware.CMake
    echo "LLVM has been installed."
) else (echo "LLVM is already installed.")

rem save the current directory in memory
pushd %CURRENT_DIR%
rem look for cmake in Program Files
cd /./Program Files
for /f "tokens=*" %%i in ('dir CMake') do set CMAKE_RESULT=%%i
rem return to CURRENT_DIR
popd
rem if cmake wasn't found, then install it
if "%CMAKE_RESULT%" == "Directory of C:\Program Files" (
    echo "Installing CMake..."
    winget install -e --id Kitware.CMake
    echo "CMake has been installed."
) else (echo "CMake is already installed.")

rem save the current directory in memory
pushd %CURRENT_DIR%
rem look for vulkan in in the root directory
cd /.
for /f "tokens=*" %%i in ('dir VulkanSDK') do set VULKAN_RESULT=%%i
rem return to CURRENT_DIR
popd
rem if vulkan wasn't found, then install it
if "%VULKAN_RESULT%" == "Directory of C:\Program Files" (
    echo "Installing Vulkan..."
    winget install -e --id KhronosGroup.VulkanSDK
    echo "Vulkan has been installed."
) else (echo "Vulkan is already installed.")


