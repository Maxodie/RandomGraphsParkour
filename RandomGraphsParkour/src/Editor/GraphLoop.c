#include "Editor/GraphLoop.h"
#include "Editor/EditorGUI.h"

#include "Application.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"

void RGP_EditorInit(RGP_EditorScene_t* scene)
{
    scene->isRunning = 1;
    scene->graphPoints = NULL;//should be initialized somewhere

    RGP_CameraInit(&scene->cam, -1.f, 10.f, scene->app->window.width, scene->app->window.height);
    ILDA_vector3f position = (ILDA_vector3f){ 2.0f, 0.0f, 0.0f };
    RGP_CameraUpdatePosition(&scene->cam, &position);
    RGP_CameraZoom(&scene->cam, 1.5f);

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

    RGP_GraphDestroy(scene);

    RGP_LOG_SUCCESS("Editor shutdowned");
}

void RGP_EditorUpdate(RGP_EditorScene_t* scene)
{
    RGP_Application_t* app = scene->app;

    static const ILDA_vector3f textColor = {0.0f, 1.0f, 0.0f};
    static const float textSize = 0.001f;
    static const float lineWidth = 0.02f;
    static const float pointWidth = 0.2f;

    RGP_RendererBegin(&app->rdrD, &scene->cam);


    for(size_t i = 0; i < scene->arcCount; ++i)
    {
        RGP_LineData_t data =
        {
            .position1 = &scene->arcs[i].start->position,
            .position2 = &scene->arcs[i].end->position,
            .width = lineWidth
        };
        RGP_RendererDrawLine(&app->rdrD, &data, &scene->arcs[i].color, scene->arcs[i].isDirected);
    }

    for(size_t i = 0; i < scene->pointCount; ++i)
    {
        RGP_PointData_t data =
        {
            .position = &scene->graphPoints[i].position,
            .size = pointWidth
        };
        RGP_RendererDrawPoint(&app->rdrD, &data, &scene->graphPoints[i].color);
    }

    //Text rendering

    for(size_t i = 0; i < scene->arcCount; ++i)
    {
        if(!scene->arcs[i].isWeigthed)
        {
            continue;
        }

        RGP_LineData_t data =
        {
            .position1 = &scene->arcs[i].start->position,
            .position2 = &scene->arcs[i].end->position,
            .width = lineWidth
        };

        char text[512];
        RGP_StringFormat(text, sizeof(text), "%d", scene->arcs[i].value);
        RGP_RendererDrawLineCenteredText(&app->rdrD, NULL, &data, &textColor, text, textSize);
    }

    for(size_t i = 0; i < scene->pointCount; ++i)
    {
        RGP_PointData_t pointData =
        {
            .position = &scene->graphPoints[i].position,
            .size = pointWidth
        };

        char text[512];
        RGP_StringFormat(text, sizeof(text), "%d", scene->graphPoints[i].value);
        RGP_RendererDrawPointCenteredText(&app->rdrD, NULL, &pointData, &textColor, text, textSize);
    }

    RGP_RendererEnd(&app->rdrD, 0);

    //setup draw stats
    scene->drawCallStatAmount = scene->app->rdrD.drawCallCountPerUpdate;
}
