# specify source and header files
set(RENDERER_SRCS 
    device.cpp
    model.cpp
    pipeline.cpp
    renderer.cpp
    swapchain.cpp
)

set(RENDERER_HDRS 
    device.h
    model.h
    object.h
    pipeline.h
    renderer.h
    swapchain.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${RENDERER_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${RENDERER_HDRS})