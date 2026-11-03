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
        .initCallback = (RGP_RoutineFunc_t)RGP_EditorInit,
        .updateCallback = (RGP_RoutineFunc_t)RGP_EditorUpdate,
        .shutdownCallback = (RGP_RoutineFunc_t)RGP_EditorShutdown,
        .data = &graphScene,
        .needsThread = 0
    };

    RGP_AppAddWorker(&app, &applicationWorker);

    RGP_AppRun(&app);
    RGP_AppShutdown(&app);

    return 0;
}
