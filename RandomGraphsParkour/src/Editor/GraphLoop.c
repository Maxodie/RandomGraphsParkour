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
    ILDA_vector3f color2 = {0.0f, 0.0f, 1.0f};

    RGP_RendererBegin(&app->rdrD, &scene->cam);


    for(size_t i = 1; i < scene->pointCount; ++i)
    {
        RGP_RendererDrawLine(&app->rdrD, &scene->graphPoints[i - 1].position, &scene->graphPoints[i].position, &color2, 0.01f, 1);
    }

    // RGP_RendererEnd(&app->rdrD, 1);
    // RGP_RendererBegin(&app->rdrD, &scene->cam);

    for(size_t i = 0; i < scene->pointCount; ++i)
    {
        RGP_RendererDrawPoint(&app->rdrD, &scene->graphPoints[i].position, &scene->graphPoints[i].color, 0.1f);
    }

    ILDA_vector3f pos = {0, 0, 0};
    RGP_RendererDrawText(&app->rdrD, &pos, &color1, "2", 0.001f);
    ILDA_vector3f pos2 = {.0f, 0.1f, 0};
    RGP_RendererDrawText(&app->rdrD, &pos2, &color2, "A", 0.005f);

    RGP_RendererEnd(&app->rdrD, 0);

    //setup draw stats
    scene->drawCallStatAmount = scene->app->rdrD.drawCallCountPerUpdate;
}
