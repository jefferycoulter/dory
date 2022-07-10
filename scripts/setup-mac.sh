#!/bin/bash

# get the current directory.
DIR="$( cd "$( dirname "$0" )" && pwd )"

# check if homebrew is installed.  if not, then install it.
if [[ $(command -v brew) == "" ]]; then
    echo "Installing Hombrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    echo "Homebrew has been installed."
else
    echo "Updating Homebrew..."
    brew update
fi

# get directory for brew
#
# homebrew places x86 binaries and ARM binaries in different locations. see:
# https://www.reddit.com/r/MacOS/comments/jw9guu/why_did_homebrew_move_from_usrlocalto_opthomebrew/
if [[ $(arch) == "arm64" ]]; then
    BPATH="/opt/homebrew/bin"
else
    BPATH="/usr/local/bin"
fi 

# make sure brew is included in the PATH.  if not, then add it. note, this will also ensure that
# cmake is in the PATH since it is located in the same directory if it has been installed with brew.
if [[ $PATH == *$BPATH* ]]; then
    echo "Homebrew is already included in the PATH."
else
    echo "Adding Homebrew to PATH..."
    export PATH="$BPATH:$PATH"
    echo "Homebrew has been added to PATH."
fi

# check if CMake is installed.  if not, then install it.
if [[ $(command -v cmake) == "" ]]; then
    echo "Installing CMake..."
    brew install cmake
    echo "CMake has been installed."
else
    echo "CMake is already installed."
fi

# check if vulkan is installed.  if not, then install it.
if [[ $(mdfind kind:folder VulkanSDK) == "" ]]; then
    echo "Downloading VulkanSDK for MacOS..."
    curl -O https://sdk.lunarg.com/sdk/download/latest/mac/vulkan-sdk.dmg
    echo "VulkanSDK for MacOS has been downloaded."
    cd $(mdfind kind:folder VulkanSDK)
    hdiutil attach vulkan-sdk.dmg && open vulkan-sdk.dmg
else
    echo "VulkanSDK for MacOS is already installed at $(mdfind kind:folder VulkanSDK)"
fi