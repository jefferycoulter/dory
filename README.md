# dory

<img width="400" alt="bunnies" src="https://user-images.githubusercontent.com/66433636/182655843-5a95841d-1b40-4bca-a387-cad1028619ef.png">

## Build
to get the repository, open a terminal or command line and run 
>`git clone --recurse-submodules -j8 https://github.com/jefferycoulter/dory.git`

the `-j8` is optional, it should just make the download faster by getting submodules in parallel.

#### MacOS
the `mac-setup.sh` is an optional script to run which will install homebrew, cmake, and vulkan if they are not already installed. to build the project, run either `./mac-build.sh` or `./mac-build-xcode.sh` if you want an xcode project. the folder structure will be weird in xcode, i need to update the cmake files.


#### Windows
the `win-setup.bat` is an optional script to run which will install clang, cmake, make, and vulkan if they are not already installed. to build the project, run `win-build`. this currently doesn't build a visual studio project, but one can be created by just making a new project from an existing directory.