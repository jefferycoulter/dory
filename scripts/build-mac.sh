#!/bin/bash

# get current direcetory
DIR="$( cd "$( dirname "$0" )" && pwd )"
VULKAN_DIR=$( cd "$(mdfind kind:folder VulkanSDK)" && cd "$(ls)" && pwd )

# write the molten-vk directory to an environment file so cmake can find it.
if [[ "$(cat $DIR/../.env.cmake)" != "set(VULKAN_SDK_PATH $VULKAN_DIR)" ]]; then
    echo "Writing CMake env file..."
    cd $DIR/.. && echo "set(VULKAN_SDK_PATH $VULKAN_DIR)" >> .env.cmake
    echo "CMake env file written."
else
    echo "CMake env file already exists."
fi

# change to build directory
cd $DIR/../build

# run cmake
cmake -S $DIR/.. -B $DIR/../build -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles"

# run make
cd $DIR/../build; VERBOSE=1 make