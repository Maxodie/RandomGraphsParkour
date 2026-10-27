#include "Core.h"
#include "Application.h"
#include "Graph/GraphLoop.h"

int main(int argc, char** argv)
{
    RGP_Application_t app;
    RGP_AppInit(&app);

    RGP_EditorScene_t graphScene; // mb make appAddRenderWorker and appAddUpdateWorker (both having the scene ref) and add a worker in each like right now and separate the update of each in a thread
    RGP_ApplicationWorker_t applicationWorker =
    {
        .InitCallback = (WorkerCallback)RGP_EditorInit,
        .UpdateCallback = (WorkerCallback)RGP_EditorUpdate,
        .ShutdownCallback = (WorkerCallback)RGP_EditorShutdown,
        .data = &graphScene
    };

    RGP_AppAddWorker(&app, &applicationWorker);

    RGP_AppRun(&app);
    RGP_AppShutdown(&app);

    return 0;
}
