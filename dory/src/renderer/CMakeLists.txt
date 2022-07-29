# specify source and header files
set(RENDERER_SRCS
    buffer.cpp
    camera.cpp
    camera_controller.cpp
    device.cpp
    descriptor.cpp
    model.cpp
    pipeline.cpp
    renderer.cpp
    swapchain.cpp
)

set(RENDERER_HDRS 
    buffer.h
    camera.h
    camera_controller.h
    data.h
    descriptor.h
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