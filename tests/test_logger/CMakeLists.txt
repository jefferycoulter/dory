project(test_logger)

# set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/bin)

# specify source and header files
set(TLOG_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test_logger.cpp)

# add the executable to be built
add_executable(${PROJECT_NAME} ${TLOG_SRCS})

# add include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dory/include)

# link the library
target_link_libraries(${PROJECT_NAME} PUBLIC dory)