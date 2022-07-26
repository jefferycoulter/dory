project(test_application)

# set the output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/bin)

# specify source and header files
set(TAPP_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test_application.cpp)

# add the executable to be built
add_executable(${PROJECT_NAME} ${TAPP_SRCS})

# add include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/dory/include)

# link the library
target_link_libraries(${PROJECT_NAME} PUBLIC dory)

# add the shaders to the test output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)