#include "Graph/GraphLoop.h"
#include "Application.h"
#include "Renderer/Renderer.h"

#include <nuklear.h>

#include <glad/glad.h>
#include "GUILayer/nuklearGlfwGL3.h"

void RGP_EditorInit(RGP_EditorScene_t* scene)
{
    scene->isRunning = 1;
    scene->graphPoints = RGP_ListInit(16, RGP_GraphPoint_t);
    scene->pointCount = 0;

    RGP_LOG_SUCCESS("Editor initialized");
}

void RGP_EditorShutdown(RGP_EditorScene_t* scene)
{
    RGP_ListDelete(scene->graphPoints);

    RGP_LOG_SUCCESS("Editor shutdowned");
}

void RGP_EditorUpdate(RGP_EditorScene_t* scene)
{
    RGP_Application_t* app = scene->app;

    RGP_RendererBegin(&app->rdrD);
    ILDA_vector3f color1 = {1.f, 0.0f, 0.5f};
    ILDA_vector3f color2 = {0.0f, 0.1f, 0.5f};

    for(size_t i = 0; i < scene->pointCount; ++i)
    {
        RGP_RendererDrawPoint(&app->rdrD, &scene->graphPoints[i].position, &scene->graphPoints[i].color);
    }

    // ILDA_vector3f location1 = {0.5f, 0.5f, 0.5f};
    // RGP_RendererDrawPoint(&app->rdrD, &location1, &color1);
    // ILDA_vector3f location2 = {-0.5f, -0.5f, -0.5f};
    // RGP_RendererDrawPoint(&app->rdrD, &location2, &color1);
    // ILDA_vector3f location3 = {0.f, 0.f, 0.f};
    // RGP_RendererDrawPoint(&app->rdrD, &location3, &color2);
}

void RGP_EditorUpdateGUI(RGP_EditorScene_t* scene)
{
    RGP_Application_t* app = scene->app;
    RGP_Window_t* ctxWindow = &app->window;

    //default example window
    enum {RED, GREEN};
        static int op = RED;
        static float value = 0.f;
     if (nk_begin(ctxWindow->guiContext, "Show", nk_rect(50, 50, 220, 220),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
            /* fixed widget pixel width */
            nk_layout_row_static(ctxWindow->guiContext, 30, 80, 1);
            if (nk_button_label(ctxWindow->guiContext, "spawn point")) {
                RGP_GraphPoint_t graphPoint = {.position = {0.0f, value, 0.0f}, .color = {0.0f, 0.0f, 0.0f}};
                if(op == RED)
                {
                    graphPoint.color.x = 1.0f;
                }
                else
                {
                    graphPoint.color.y = 1.0f;
                }
                RGP_ListAddCpy(scene->graphPoints, &graphPoint);
                ++scene->pointCount;
                RGP_LOG_DEBUG("point added");
                /* event handling */
            }

            /* fixed widget window ratio width */
            nk_layout_row_dynamic(ctxWindow->guiContext, 30, 2);
            if (nk_option_label(ctxWindow->guiContext, "Red", op == RED)) op = RED;
            if (nk_option_label(ctxWindow->guiContext, "Green", op == GREEN)) op = GREEN;

            /* custom widget pixel width */
            nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctxWindow->guiContext, 110);
                nk_label(ctxWindow->guiContext, "Point Height : ", NK_TEXT_LEFT);
            }
            nk_layout_row_end(ctxWindow->guiContext);

            nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
            {
                nk_layout_row_push(ctxWindow->guiContext, 110);
                nk_slider_float(ctxWindow->guiContext, -1.0f, &value, 1.0f, 0.1f);
            }
            nk_layout_row_end(ctxWindow->guiContext);
        }
        nk_end(ctxWindow->guiContext);
    //----
}
