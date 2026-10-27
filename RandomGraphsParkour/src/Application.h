#pragma once
#include "Window/Window.h"
#include <Renderer/Renderer.h>

typedef struct RGP_ApplicationWorker_s
{
    WorkerCallback InitCallback;
    WorkerCallback UpdateCallback;
    WorkerCallback ShutdownCallback;
    void* data;
} RGP_ApplicationWorker_t;

struct RGP_Application_s
{
    RGP_Window_t window;
    RGP_RendererData_t rdrD;

    RGP_ApplicationWorker_t* workerList;
    size_t workerCount;

    Bool isRunning;
};

void RGP_AppInit(RGP_Application_t* app);
void RGP_AppShutdown(RGP_Application_t* app);

void RGP_AppHandleEvent(RGP_Application_t* app, RPG_WindowEventType eventType);

void RGP_AppInitWorkerList(RGP_Application_t* app);
//DO NOT ADD WORKER DURING UPDATE EXECUTION
void RGP_AppAddWorker(RGP_Application_t* app, const RGP_ApplicationWorker_t* worker);
void RGP_AppShutdownWorkerList(RGP_Application_t* app);

void RGP_AppRun(RGP_Application_t* app);
void RGP_AppStop(RGP_Application_t* app);
