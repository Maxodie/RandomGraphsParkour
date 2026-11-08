#include "Application.h"
#include "GUILayer/GUILayer.h"
#include "Editor/GraphLoop.h"
#include "Utils/ThreadPool.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void RGP_AppInit(RGP_Application_t* app)
{
    RGP_ASSERT(app != NULL, "app should not be NULL");

    if(!RGP_WindowInitializeAPI())
    {
        RGP_ASSERT(0, "API initialization failed");
        return;
    }
    RGP_LOG_SUCCESS("Application initialized");


    if(!RGP_WindowCreate(&app->window, app, 1200, 720, "RGP", RGP_AppHandleEvent))
    {
        RGP_ASSERT(0, "Window creation failed");
        return;
    }

    RGP_RendererInit(&app->rdrD, &app->window);
    RGP_GuiInit(&app->window);

    app->threadPool = RGP_ThreadCreate(RGP_ThreadMaxAmount());

    RGP_AppInitWorkerList(app);

    app->isRunning = 1;
}

void RGP_AppShutdown(RGP_Application_t* app)
{
    RGP_ASSERT(app != NULL, "app should not be NULL");
    RGP_AppShutdownWorkerList(app);

    RGP_ThreadDestroy(app->threadPool);

    RGP_GuiShutdown();

    RGP_RendererShutdown(&app->rdrD);

    RGP_WindowDestroy(&app->window);

    RGP_LOG_SUCCESS("Application shutdowned");
}

void RGP_AppHandleEvent(RGP_Application_t* app, RPG_WindowEventType eventType)
{
    if(eventType == RPG_WINDOW_EVENT_TYPE_CLOSE)
    {
        RGP_AppStop(app);
    }
}

void RGP_AppInitWorkerList(RGP_Application_t* app)
{
    RGP_ASSERT(app != NULL, "app should not be NULL");

    app->workerList = RGP_ListInit(4, RGP_ApplicationWorker_t);
    app->workerCount = 0;

    app->taskList = RGP_ListInit(4, RGP_ApplicationTask_t);
    app->taskCount = 0;

    RGP_LOG_SUCCESS("App workers and tasks initialized");
}

void RGP_AppAddWorker(RGP_Application_t* app, const RGP_ApplicationWorker_t* worker)
{
    RGP_ASSERT(app != NULL && worker != NULL, "app and worker should not be NULL");

    RGP_ListAddCpy(app->workerList, worker);
    ++app->workerCount;

    if(worker->initCallback)
    {
        if(worker->needsThread)
        {
            RGP_ThreadAddWork(app->threadPool, worker->initCallback, worker->data);
        }
        else
        {
            worker->initCallback(worker->data); //needs to be last in case of recursive additional worker
        }
    }
}

void RGP_AppAddTask(RGP_Application_t* app, const RGP_ApplicationTask_t* task)
{
    RGP_ASSERT(app != NULL && task != NULL, "app and task should not be NULL");

    RGP_ListAddCpy(app->taskList, task);
    ++app->taskCount;
}

void RGP_AppShutdownWorkerList(RGP_Application_t* app)
{
    RGP_ASSERT(app != NULL, "app should not be NULL");

    RGP_ListDelete(app->taskList);
    app->taskCount = 0;

    for(int32_t i = app->workerCount - 1; i >= 0; --i)//shutdown in LIFO
    {
        RGP_ApplicationWorker_t* worker = &app->workerList[i];
        if(worker->shutdownCallback)
        {
            if(worker->needsThread)
            {
                RGP_ThreadAddWork(app->threadPool, worker->initCallback, worker->data);
            }
            else
            {
                worker->initCallback(worker->data); //needs to be last in case of recursive additional worker
            }
        }
    }

    app->workerCount = 0;
    RGP_ListDelete(app->workerList);

    RGP_LOG_SUCCESS("App workers shutdowned");
}

void RGP_AppProcessWorkers(RGP_Application_t* app)
{
    for(size_t i = 0; i < app->workerCount; ++i)
    {
        RGP_ApplicationWorker_t* worker = &app->workerList[i];
        if(worker->updateCallback)
        {
            if(worker->needsThread)
            {
                RGP_ThreadAddWork(app->threadPool, (RGP_RoutineFunc_t)worker->updateCallback, worker->data);
            }
            else
            {
                worker->updateCallback(worker->data);
            }
        }
    }
}

void RGP_AppProcessTasks(RGP_Application_t* app)
{
    if(app->taskCount == 0)
    {
        return;
    }

    for(size_t i = 0; i < app->taskCount; --i)
    {
        RGP_ApplicationTask_t* worker = &app->taskList[i];
        if(worker->callback)
        {
            if(worker->needsThread)
            {
                RGP_ThreadAddWork(app->threadPool, (RGP_RoutineFunc_t)worker->callback, worker->data);
            }
            else
            {
                worker->callback(worker->data);
            }
        }
    }

    app->taskCount = 0;
    RGP_ListRemoveLast(app->taskList);
}

void RGP_AppRun(RGP_Application_t* app)
{
    RGP_LOG_SUCCESS("Application started");
    while(app->isRunning)
    {
        RGP_WindowPoll();

        RGP_RendererClear(&app->rdrD);
        RGP_GuiBegin();

        RGP_AppProcessWorkers(app);


        RGP_ThreadWait(app->threadPool);
        RGP_GuiEnd(&app->window);

        RGP_WindowSwapBuffers(&app->window);

        RGP_AppProcessTasks(app);
    }
}

void RGP_AppStop(RGP_Application_t* app)
{
    RGP_LOG_SUCCESS("Application stopped");
    app->isRunning = 0;
}
