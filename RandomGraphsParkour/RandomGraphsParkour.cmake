add_executable(RandomGraphsParkour

    RandomGraphsParkour/src/Application.h
    RandomGraphsParkour/src/Application.c

    RandomGraphsParkour/src/EntryPoint.c
    RandomGraphsParkour/src/Core.h

    RandomGraphsParkour/src/Log/Log.h
    RandomGraphsParkour/src/Log/Log.c

    RandomGraphsParkour/src/Window/Window.h
    RandomGraphsParkour/src/Window/Window.c

    RandomGraphsParkour/src/Renderer/Vertex.h
    RandomGraphsParkour/src/Renderer/Renderer.h
    RandomGraphsParkour/src/Renderer/Renderer.c
    RandomGraphsParkour/src/Renderer/Camera.h
    RandomGraphsParkour/src/Renderer/Camera.c
    RandomGraphsParkour/src/Renderer/Text.h
    RandomGraphsParkour/src/Renderer/Text.c

    RandomGraphsParkour/src/Utils/List.h
    RandomGraphsParkour/src/Utils/List.c
    RandomGraphsParkour/src/Utils/String.h
    RandomGraphsParkour/src/Utils/String.c
    RandomGraphsParkour/src/Utils/ThreadPool.h
    RandomGraphsParkour/src/Utils/ThreadPool.c

    RandomGraphsParkour/src/Editor/GraphData.h
    RandomGraphsParkour/src/Editor/GraphLoop.h
    RandomGraphsParkour/src/Editor/GraphLoop.c
    RandomGraphsParkour/src/Editor/EditorGUI.h
    RandomGraphsParkour/src/Editor/EditorGUI.c

    RandomGraphsParkour/src/GUILayer/GUILayer.h
    RandomGraphsParkour/src/GUILayer/GUILayer.c
    RandomGraphsParkour/src/GUILayer/nuklearGlfwGL3.h
)

target_link_libraries(RandomGraphsParkour PRIVATE
    glfw
    glad
    Mathilda
    freetype
)

target_include_directories(RandomGraphsParkour PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/GLFW/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/glad/glad/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/nuklear/nuklear
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/Mathilda/Mathilda/Mathilda/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RandomGraphsParkour/vendors/FreeType/freetype/include
)

set_target_properties(RandomGraphsParkour PROPERTIES
    OUTPUT_NAME RandomGraphsParkour.out
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/RandomGraphsParkour
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/RandomGraphsParkour
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/RandomGraphsParkour
)
