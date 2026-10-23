add_library( glad
    RandomGraphsParkour/vendors/glad/glad/src/glad.c
    RandomGraphsParkour/vendors/glad/glad/include/glad/glad.h
    RandomGraphsParkour/vendors/glad/glad/include/KHR/khrplatform.h
)

target_compile_options(glad PRIVATE
    -o3
)

set_target_properties(glad PROPERTIES
    OUTPUT_NAME glad
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/glad
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/glad
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/glad
)

target_include_directories(glad PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/glad/glad/include
)
