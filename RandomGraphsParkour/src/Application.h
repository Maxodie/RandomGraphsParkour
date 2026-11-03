#pragma once
#include "Core.h"
#include "Window/Window.h"
#include <Renderer/Renderer.h>

typedef struct RGP_ApplicationWorker_s
{
    RGP_RoutineFunc_t initCallback;
    RGP_RoutineFunc_t updateCallback;
    RGP_RoutineFunc_t shutdownCallback;
    void* data;
    Bool needsThread;
} RGP_ApplicationWorker_t;

typedef struct RGP_ApplicationTask_s
{
    RGP_RoutineFunc_t callback;
    void* data;
    Bool needsThread;
} RGP_ApplicationTask_t;

struct RGP_Application_s
{
    RGP_Window_t window;
    RGP_RendererData_t rdrD;

    RGP_ApplicationWorker_t* workerList;
    RGP_ApplicationTask_t* taskList;
    size_t workerCount;
    size_t taskCount;

    RGP_ThreadPool_t* threadPool;

    Bool isRunning;
};

void RGP_AppInit(RGP_Application_t* app);
void RGP_AppShutdown(RGP_Application_t* app);

void RGP_AppHandleEvent(RGP_Application_t* app, RPG_WindowEventType eventType);

void RGP_AppInitWorkerList(RGP_Application_t* app);
//DO NOT ADD WORKER DURING UPDATE EXECUTION
void RGP_AppAddWorker(RGP_Application_t* app, const RGP_ApplicationWorker_t* worker);
void RGP_AppAddTask(RGP_Application_t* app, const RGP_ApplicationTask_t* task);
void RGP_AppShutdownWorkerList(RGP_Application_t* app);

void RGP_AppProcessWorkers(RGP_Application_t* app);
void RGP_AppProcessTasks(RGP_Application_t* app);

void RGP_AppRun(RGP_Application_t* app);
void RGP_AppStop(RGP_Application_t* app);
