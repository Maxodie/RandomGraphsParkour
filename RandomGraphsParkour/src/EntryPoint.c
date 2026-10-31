#include "Core.h"
#include "Application.h"
#include "Graph/GraphLoop.h"

int main(int argc, char** argv)
{
    RGP_Application_t app;
    RGP_AppInit(&app);

    RGP_EditorScene_t graphScene;
    graphScene.app = &app;
    RGP_ApplicationWorker_t applicationWorker =
    {
        .InitCallback = (RGP_RoutineFunc_t)RGP_EditorInit,
        .UpdateCallback = (RGP_RoutineFunc_t)RGP_EditorUpdate,
        .ShutdownCallback = (RGP_RoutineFunc_t)RGP_EditorShutdown,
        .data = &graphScene,
    };

    RGP_AppAddWorker(&app, &applicationWorker);

    RGP_ApplicationWorker_t applicationWorkerGUI =
    {
        .InitCallback = NULL,
        .UpdateCallback = (RGP_RoutineFunc_t)RGP_EditorUpdateGUI,
        .ShutdownCallback = NULL,
        .data = &graphScene,
    };
    RGP_AppAddWorker(&app, &applicationWorkerGUI);


    RGP_AppRun(&app);
    RGP_AppShutdown(&app);

    return 0;
}
