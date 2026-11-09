#include "Editor/GraphLoop.h"
#include "Editor/EditorGUI.h"

#include "Application.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"

void RGP_EditorInit(RGP_EditorScene_t* scene)
{
    scene->isRunning = 1;
    scene->graphPoints = RGP_ListInit(16, RGP_GraphPoint_t);
    scene->pointCount = 0;

    RGP_CameraInit(&scene->cam, -1.f, 10.f, scene->app->window.width, scene->app->window.height);
    ILDA_vector3f position = (ILDA_vector3f){ 0.0f, 0.0f, -1.0f };
    RGP_CameraUpdatePosition(&scene->cam, &position);

    RGP_ApplicationWorker_t applicationWorkerGUI =
    {
        .initCallback = NULL,
        .updateCallback = (RGP_RoutineFunc_t)RGP_EditorUpdateGUI,
        .shutdownCallback = NULL,
        .data = scene,
        .needsThread = 1
    };
    RGP_AppAddWorker(scene->app, &applicationWorkerGUI);

    RGP_LOG_SUCCESS("Editor initialized");
}

void RGP_EditorShutdown(RGP_EditorScene_t* scene)
{
    RGP_CameraShutdown(&scene->cam);
    RGP_ListDelete(scene->graphPoints);

    RGP_LOG_SUCCESS("Editor shutdowned");
}

void RGP_EditorUpdate(RGP_EditorScene_t* scene)
{
    RGP_Application_t* app = scene->app;

    ILDA_vector3f color1 = {1.f, 0.0f, 0.5f};
    ILDA_vector3f color2 = {0.0f, 1.0f, 0.0f};

    RGP_RendererBegin(&app->rdrD, &scene->cam);


    for(size_t i = 1; i < scene->pointCount; ++i)
    {
        RGP_LineData_t data =
        {
            .position1 = &scene->graphPoints[i - 1].position,
            .position2 = &scene->graphPoints[i].position,
            .width = 0.01f
        };
        RGP_RendererDrawLine(&app->rdrD, &data, &color1, 1);
    }

    // RGP_RendererEnd(&app->rdrD, 1);
    // RGP_RendererBegin(&app->rdrD, &scene->cam);

    for(size_t i = 0; i < scene->pointCount; ++i)
    {
        RGP_PointData_t data =
        {
            .position = &scene->graphPoints[i].position,
            .size = 0.3f
        };
        RGP_RendererDrawPoint(&app->rdrD, &data, &scene->graphPoints[i].color);
    }

    for(size_t i = 0; i < scene->pointCount; ++i)
    {
        RGP_PointData_t pointData =
        {
            .position = &scene->graphPoints[i].position,
            .size = 0.3f
        };
        RGP_RendererDrawPointCenteredText(&app->rdrD, NULL, &pointData, &color2, "5", 0.002f);

        if(i > 0)
        {
            RGP_LineData_t lineData =
            {
                .position1 = &scene->graphPoints[i - 1].position,
                .position2 = &scene->graphPoints[i].position,
                .width = 0.3f
            };
            RGP_RendererDrawLineCenteredText(&app->rdrD, NULL, &lineData, &color2, "325", 0.002f);
        }
    }

    RGP_RendererEnd(&app->rdrD, 0);

    //setup draw stats
    scene->drawCallStatAmount = scene->app->rdrD.drawCallCountPerUpdate;
}
