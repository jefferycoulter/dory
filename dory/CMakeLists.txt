project(dory)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dory)

add_library(${PROJECT_NAME} dory.h)

add_subdirectory(src)

# link libraries
target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        ${Vulkan_LIBRARIES}
        glfw # glfw needs to be lowercase
        glm
)

# specify include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src

        ${Vulkan_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/ext/GLFW/include 
        ${CMAKE_SOURCE_DIR}/ext/glm  
)