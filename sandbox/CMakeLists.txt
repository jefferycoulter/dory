project(sandbox)

# set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/sandbox/bin)

# specify source and header files
file(GLOB SANDBOX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB SANDBOX_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SANDBOX_SRCS} ${SANDBOX_HDRS})

# link the library
target_link_libraries(${PROJECT_NAME} PUBLIC dory)
