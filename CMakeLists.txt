# set cmake version
cmake_minimum_required(VERSION 3.16)

# project name
project(DORY)

# cmake settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# compiler settings
add_compile_options(-g -Wall -Wextra)

# glfw settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# find the vulkan sdk
find_package(Vulkan REQUIRED)
if (VULKAN_FOUND)
    message(STATUS "Found Vulkan")
endif (VULKAN_FOUND)

# specify directories with cmake files
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
add_subdirectory(dory)
add_subdirectory(tests)

#add_subdirectory(sandbox) # not part of the engine, should be made optional eventually
