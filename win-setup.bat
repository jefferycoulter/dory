@echo off

rem get current directory
set CURRENT_DIR=%cd%

rem check for llvm/clang
pushd %CURRENT_DIR%
cd /./Program Files
for /f "tokens=*" %%i in ('dir LLVM') do set LLVM_RESULT=%%i
popd
if "%LLVM%" == "Directory of C:\Program Files" (
    echo "Installing LLVM..."
    winget install -e --id Kitware.CMake
    echo "LLVM has been installed."
) else (echo "LLVM is already installed.")

rem check for cmake
pushd %CURRENT_DIR%
cd /./Program Files
for /f "tokens=*" %%i in ('dir CMake') do set CMAKE_RESULT=%%i
popd
if "%CMAKE_RESULT%" == "Directory of C:\Program Files" (
    echo "Installing CMake..."
    winget install -e --id Kitware.CMake
    echo "CMake has been installed."
) else (echo "CMake is already installed.")

rem check for gnu make
pushd %CURRENT_DIR%
cd /./Program Files
for /f "tokens=*" %%i in ('dir GnuWin32') do set GNU_RESULT=%%i
popd
if "%GNU_RESULT%" == "Directory of C:\Program Files" (
    echo "Installing GNU Make..."
    winget install -e --id GnuWin32.Make
    echo "GNU Make has been installed."
) else (echo "GNU Make is already installed.")

rem check for vulkan
pushd %CURRENT_DIR%
cd /.
for /f "tokens=*" %%i in ('dir VulkanSDK') do set VULKAN_RESULT=%%i
popd
if "%VULKAN_RESULT%" == "Directory of C:\Program Files" (
    echo "Installing Vulkan..."
    winget install -e --id KhronosGroup.VulkanSDK
    echo "Vulkan has been installed."
) else (echo "Vulkan is already installed.")


