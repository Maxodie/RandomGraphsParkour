#include "GUILayer/GUILayer.h"
#include "Application.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION

#include <nuklear.h>

#include <glad/glad.h>

//taken from the demo folder and modified since the opengl3 exemple were taking
//the glfw user point for itself
#include "GUILayer/nuklearGlfwGL3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

void RGP_GuiInit(RGP_Window_t* ctxWindow)
{
    RGP_ASSERT(ctxWindow, "The window ctx should not be NULL");
    RGP_ASSERT(ctxWindow->native, "The native window ctx should not be NULL");

    ctxWindow->guiContext = nk_glfw3_init(
            (GLFWwindow*)ctxWindow->native, NK_GLFW3_INSTALL_CALLBACKS
            );

    struct nk_font_atlas* atlas;
	nk_glfw3_font_stash_begin(&atlas);
	nk_glfw3_font_stash_end();

    RGP_LOG_SUCCESS("GUI Initialized");
}

void RGP_GuiShutdown()
{
    nk_glfw3_shutdown();
    RGP_LOG_SUCCESS("GUI shutdowned");
}

void RGP_GuiBegin()
{
    nk_glfw3_new_frame();
}

void RGP_GuiEnd(RGP_Window_t* ctxWindow)
{
    nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}
