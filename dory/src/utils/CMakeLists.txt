# specify source and header files
set(UTILS_SRCS 
    utils.cpp
)

set(UTILS_HDRS 
    nocopy.h
    utils.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${UTILS_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${UTILS_HDRS})