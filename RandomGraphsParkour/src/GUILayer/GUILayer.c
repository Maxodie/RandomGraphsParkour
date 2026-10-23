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
    RGP_ASSERT(ctxWindow, "RGP GUI Init, the window ctx should not be NULL");
    RGP_ASSERT(ctxWindow->native, "RGP GUI Init, the native window ctx should not be NULL");

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
    enum {EASY, HARD};
        static int op = EASY;
        static float value = 0.6f;
     if (nk_begin(ctxWindow->guiContext, "Show", nk_rect(50, 50, 220, 220),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
            /* fixed widget pixel width */
            nk_layout_row_static(ctxWindow->guiContext, 30, 80, 1);
            if (nk_button_label(ctxWindow->guiContext, "button")) {
                /* event handling */
            }

            /* fixed widget window ratio width */
            nk_layout_row_dynamic(ctxWindow->guiContext, 30, 2);
            if (nk_option_label(ctxWindow->guiContext, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctxWindow->guiContext, "hard", op == HARD)) op = HARD;

            /* custom widget pixel width */
            nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctxWindow->guiContext, 50);
                nk_label(ctxWindow->guiContext, "Volume:", NK_TEXT_LEFT);
                nk_layout_row_push(ctxWindow->guiContext, 110);
                nk_slider_float(ctxWindow->guiContext, 0, &value, 1.0f, 0.1f);
            }
            nk_layout_row_end(ctxWindow->guiContext);
        }
        nk_end(ctxWindow->guiContext);

    nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}
