# specify source and header files
set(RENDERER_SRCS 
    device.cpp
    pipeline.cpp
)

set(RENDERER_HDRS 
    device.h
    pipeline.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${RENDERER_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${RENDERER_HDRS})