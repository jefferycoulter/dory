# specify source and header files
set(CORE_SRCS

)
set(CORE_HDRS
    hash.h
    push.h
    transforms.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${CORE_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${CORE_HDRS})