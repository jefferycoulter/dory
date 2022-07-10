# specify source and header files
file(GLOB CORE_SRCS *.cpp)
file(GLOB CORE_HDRS *.h)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${CORE_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${CORE_HDRS})