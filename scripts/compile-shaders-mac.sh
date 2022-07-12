#!/bin/bash

# get necessary directories
DIR="$( cd "$( dirname "$0" )" && pwd )"
VULKAN_DIR=$( cd "$(mdfind kind:folder VulkanSDK)" && cd "$(ls)" && pwd )

# get compiler
GLSLC=$VULKAN_DIR/macOS/bin/glslc

# compile shader programs
$GLSLC $DIR/../shaders/shader.vert -o $DIR/../shaders/shader.vert.spv
$GLSLC $DIR/../shaders/shader.frag -o $DIR/../shaders/shader.frag.spv