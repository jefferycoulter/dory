@echo OFF

rem get current directory
set CURRENT_DIR=%cd%
rem get vulkan directory
pushd %CURRENT_DIR%
rem compile the shaders
echo "Compiling shaders..."
call scripts/compile-shaders-win.bat
echo "Shaders compiled."
rem run cmake
cmake -S %CURRENT_DIR% -B %CURRENT_DIR%\build ^
        -G "Unix Makefiles" ^
        -DCMAKE_CXX_COMPILER=clang++ ^
        -DCMAKE_C_COMPILER=clang ^
        -DCMAKE_MAKE_PROGRAM="C:\\Program Files (x86)\\GnuWin32\\bin\\make.exe" ^
        -DCMAKE_BUILD_TYPE=Debug

cd build
make
pause