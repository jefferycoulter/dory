# specify source and header files
set(CORE_SRCS
    application.cpp
    logger.cpp
)
set(CORE_HDRS
    application.h
    core.h
    entry.h
    logger.h
    timer.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${CORE_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${CORE_HDRS})