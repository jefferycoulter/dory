#!/bin/bash

# get current direcetory
DIR="$( cd "$( dirname "$0" )" && pwd )"
VULKAN_DIR=$( cd "$(mdfind kind:folder VulkanSDK)" && cd "$(ls)" && pwd )

# write the molten-vk directory to an environment file so cmake can find it.
if [[ "$(cat $DIR/.env.cmake)" != "set(VULKAN_SDK_PATH $VULKAN_DIR)" ]]; then
    echo "Writing CMake environment file..."
    cd $DIR && echo "set(VULKAN_SDK_PATH $VULKAN_DIR)" >> .env.cmake
    echo "CMake environment file written."
else
    echo "CMake environment file already exists."
fi

# compile the shaders
echo "Compiling shaders..."
scripts/compile-shaders-mac.sh
echo "Shaders compiled."

# run cmake
cmake -S $DIR -B $DIR/build \
    -G "Unix Makefiles" \
    -DCMAKE_C_COMPILER="/usr/bin/clang" \
    -DCMAKE_CXX_COMPILER="/usr/bin/clang++" \
    -DCMAKE_BUILD_TYPE=Debug

# tell make to run on multiple threads if possible
if [[ -z "$MAKEFLAGS" ]]
then
      export MAKEFLAGS=-j$(sysctl -n hw.ncpu)
fi

# run make
cd $DIR/build; VERBOSE=1 make