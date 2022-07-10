# specify source and header files
file(GLOB PLATFORM_SRCS *.cpp)
file(GLOB PLATFORM_HDRS *.h)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${PLATFORM_HDRS})