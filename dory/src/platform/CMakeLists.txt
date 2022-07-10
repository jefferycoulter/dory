# specify source and header files
set(PLATFORM_SRCS
    window.cpp
)

set(PLATFORM_HDRS 
    window.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${PLATFORM_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${PLATFORM_HDRS})