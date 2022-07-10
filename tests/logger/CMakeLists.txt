project(tlog)

# set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/logger)

# specify source and header files
file(GLOB TLOG_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB TLOG_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# add the executable to be built
add_executable(${PROJECT_NAME} ${TLOG_SRCS} ${TLOG_HDRS})

# link the library
target_link_libraries(${PROJECT_NAME} PUBLIC dory)