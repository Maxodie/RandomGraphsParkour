#include "Editor/EditorGUI.h"
#include "Editor/GraphData.h"
#include "Renderer/Camera.h"
#include <nuklear.h>

void RGP_EditorUpdateGUI(RGP_EditorScene_t* scene)
{
    RGP_Application_t* app = scene->app;
    RGP_Window_t* ctxWindow = &app->window;

    //default example window
    enum {RED, GREEN};
    static int op = RED;
    static float value = 0.f;
    static float value2 = 0.f;
    if (nk_begin(ctxWindow->guiContext, "Show", nk_rect(50, 50, 250, 470),
                NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
        /* fixed widget pixel width */
        nk_layout_row_static(ctxWindow->guiContext, 30, 120, 1);
        if (nk_button_label(ctxWindow->guiContext, "Generate graph"))
        {
            RGP_ApplicationTask_t task =
            {
                .data = scene,
                .callback = (RGP_RoutineFunc_t)RGP_GraphGeneratorProcess,
                .needsThread = 1,
            };
            RGP_AppAddTask(scene->app, &task);
        }

        if (nk_button_label(ctxWindow->guiContext, "spawn point")) {
            // RGP_GraphPoint_t graphPoint = {.position = {value2, value, 0.0f}, .color = {0.0f, 0.0f, 0.0f}};
            RGP_GraphPoint_t graphPoint = {.position = {value2, value, 0.0f}, .color = {0.0f, 0.0f, 0.0f}};
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

        nk_layout_row_static(ctxWindow->guiContext, 30, 120, 1);
        nk_layout_row_static(ctxWindow->guiContext, 30, 120, 1);
        if(scene->graphPoints != NULL)
        {
            if (nk_button_label(ctxWindow->guiContext, "Clear graph"))
            {
                RGP_ApplicationTask_t task =
                {
                    .data = scene,
                    .callback = (RGP_RoutineFunc_t)RGP_GraphDestroy,
                    .needsThread = 1,
                };
                RGP_AppAddTask(scene->app, &task);
            }
        }
        /* fixed widget window ratio width */
        nk_layout_row_dynamic(ctxWindow->guiContext, 30, 2);
        if (nk_option_label(ctxWindow->guiContext, "Red", op == RED)) op = RED;
        if (nk_option_label(ctxWindow->guiContext, "Green", op == GREEN)) op = GREEN;

        /* custom widget pixel width */
        nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctxWindow->guiContext, 110);
            nk_label(ctxWindow->guiContext, "Point vertical : ", NK_TEXT_LEFT);
            nk_slider_float(ctxWindow->guiContext, -1.0f, &value, 1.0f, 0.1f);
        }
        nk_layout_row_end(ctxWindow->guiContext);

        //custom x pos
        nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctxWindow->guiContext, 110);
            nk_label(ctxWindow->guiContext, "Point horizontal : ", NK_TEXT_LEFT);
            nk_slider_float(ctxWindow->guiContext, -1.0f, &value2, 1.0f, 0.1f);
        }
        nk_layout_row_end(ctxWindow->guiContext);

        //Cam controller
        ILDA_vector3f NewPos = scene->cam.position;
        nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctxWindow->guiContext, 110);
            nk_label(ctxWindow->guiContext, "Cam X : ", NK_TEXT_LEFT);
            nk_property_float(ctxWindow->guiContext, "#Xcam", -FLT_MAX, &NewPos.x, FLT_MAX, 0.1f, 0.1f);
        }
        nk_layout_row_end(ctxWindow->guiContext);
        nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctxWindow->guiContext, 110);
            nk_label(ctxWindow->guiContext, "Cam Y : ", NK_TEXT_LEFT);
            nk_property_float(ctxWindow->guiContext, "#Ycam", -FLT_MAX, &NewPos.y, FLT_MAX, 0.1f, 0.1f);
        }
        nk_layout_row_end(ctxWindow->guiContext);

        if(NewPos.x != scene->cam.position.x || NewPos.y != scene->cam.position.y)
        {
            RGP_CameraUpdatePosition(&scene->cam, &NewPos);
        }

        float maxZoom = 10.f;
        float zoomLevel = maxZoom - scene->cam.zoom;
        nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctxWindow->guiContext, 110);
            nk_label(ctxWindow->guiContext, "Zoom : ", NK_TEXT_LEFT);
            nk_slider_float(ctxWindow->guiContext, 0.0f, &zoomLevel, maxZoom - 0.1f, 0.1f);
        }
        nk_layout_row_end(ctxWindow->guiContext);

        zoomLevel = maxZoom - zoomLevel;
        if(zoomLevel != scene->cam.zoom)
        {
            RGP_CameraZoom(&scene->cam, zoomLevel);
        }

        //Stats
        nk_layout_row_begin(ctxWindow->guiContext, NK_STATIC, 30, 2);
        {
            char drawCallAmount[512];
            RGP_StringFormat(drawCallAmount, 512, "draw call : %lld", scene->drawCallStatAmount);
            nk_layout_row_push(ctxWindow->guiContext, 110);
            nk_label(ctxWindow->guiContext, drawCallAmount, NK_TEXT_LEFT);
        }
        nk_layout_row_end(ctxWindow->guiContext);
    }
    nk_end(ctxWindow->guiContext);
    //----
}
