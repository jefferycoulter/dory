# specify source and header files
set(CORE_SRCS

)
set(CORE_HDRS
    event.h
    keyboard_event.h
    mouse_event.h
    window_event.h    
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${CORE_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${CORE_HDRS})