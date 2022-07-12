#!/bin/bash

# get necessary directories
DIR="$( cd "$( dirname "$0" )" && pwd )"
VULKAN_DIR=$( cd "$(mdfind kind:folder VulkanSDK)" && cd "$(ls)" && pwd )

# get compiler
GLSLC=$VULKAN_DIR/macOS/bin/glslc

# compile shader programs
$GLSLC $DIR/../assets/shaders/shader.vert -o $DIR/../assets/shaders/shader.vert.spv
$GLSLC $DIR/../assets/shaders/shader.frag -o $DIR/../assets/shaders/shader.frag.spv