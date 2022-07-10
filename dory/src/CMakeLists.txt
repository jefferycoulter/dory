# specify source and header files
file(GLOB DORY_SRCS *.cpp)
file(GLOB DORY_HDRS *.h)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${DORY_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${DORY_HDRS})

add_subdirectory(core)