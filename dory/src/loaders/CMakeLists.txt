# specify source and header files
set(LOADERS_SRCS
    object_loader.cpp
)
set(LOADERS_HDRS
    object_loader.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${LOADERS_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${LOADERS_HDRS})