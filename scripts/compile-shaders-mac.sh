#!/bin/bash

# get necessary directories
DIR="$( cd "$( dirname "$0" )" && pwd )"
VULKAN_DIR=$( cd "$(mdfind kind:folder VulkanSDK)" && cd "$(ls)" && pwd )

# get compiler
GLSLC=$VULKAN_DIR/macOS/bin/glslc

# compile shader programs
cd $DIR/../assets/shaders
for FILE in *.{vert,frag}
    do echo "Compiling $FILE."; $GLSLC $FILE -o $FILE.spv; echo "$FILE.spv created."
done
cd $DIR