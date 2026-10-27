add_executable(RandomGraphsParkour

    RandomGraphsParkour/src/Application.h
    RandomGraphsParkour/src/Application.c

    RandomGraphsParkour/src/EntryPoint.c
    RandomGraphsParkour/src/Core.h

    RandomGraphsParkour/src/Log/Log.h
    RandomGraphsParkour/src/Log/Log.c

    RandomGraphsParkour/src/Window/Window.h
    RandomGraphsParkour/src/Window/Window.c

    RandomGraphsParkour/src/Renderer/Renderer.h
    RandomGraphsParkour/src/Renderer/Renderer.c

    RandomGraphsParkour/src/Utils/List.h
    RandomGraphsParkour/src/Utils/List.c

    RandomGraphsParkour/src/Graph/GraphData.h
    RandomGraphsParkour/src/Graph/GraphLoop.h
    RandomGraphsParkour/src/Graph/GraphLoop.c

    RandomGraphsParkour/src/GUILayer/GUILayer.h
    RandomGraphsParkour/src/GUILayer/GUILayer.c
    RandomGraphsParkour/src/GUILayer/nuklearGlfwGL3.h
)

target_link_libraries(RandomGraphsParkour PRIVATE
    glfw
    glad
    Mathilda
)

target_include_directories(RandomGraphsParkour PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/GLFW/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/glad/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/nuklear/nuklear
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/Mathilda/Mathilda/Mathilda/include
)

set_target_properties(RandomGraphsParkour PROPERTIES
    OUTPUT_NAME RandomGraphsParkour.out
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/RandomGraphsParkour
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/RandomGraphsParkour
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/RandomGraphsParkour
)
