# specify source and header files
set(SYSTEMS_SRCS 
    renderer_system.cpp
)

set(SYSTEMS_HDRS 
    renderer_system.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${SYSTEMS_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${SYSTEMS_HDRS})