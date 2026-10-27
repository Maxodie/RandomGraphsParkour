#include "Application.h"
#include "Log/Log.h"
#include "GUILayer/GUILayer.h"
#include "Graph/GraphLoop.h"

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


    if(!RGP_WindowCreate(&app->window, app, 840, 600, "RGP", RGP_AppHandleEvent))
    {
        RGP_ASSERT(0, "Window creation failed");
        return;
    }

    RGP_RendererInit(&app->rdrD);
    RGP_GuiInit(&app->window);

    RGP_AppInitWorkerList(app);

    app->isRunning = 1;
}

void RGP_AppShutdown(RGP_Application_t* app)
{
    RGP_ASSERT(app != NULL, "app should not be NULL");
    RGP_AppShutdownWorkerList(app);

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

    RGP_LOG_SUCCESS("App workers initialized");
}

void RGP_AppAddWorker(RGP_Application_t* app, const RGP_ApplicationWorker_t* worker)
{
    RGP_ASSERT(app != NULL && worker != NULL, "app and worker should not be NULL");

    RGP_ListAddCpy(app->workerList, worker);
    ++app->workerCount;

    if(worker->ShutdownCallback)
    {
        worker->InitCallback(app, worker->data); //needs to be last in case of recursive additional worker
    }
}

void RGP_AppShutdownWorkerList(RGP_Application_t* app)
{
    RGP_ASSERT(app != NULL, "app should not be NULL");

    for(int32_t i = app->workerCount - 1; i >= 0; --i)//shutdown in LIFO
    {
        RGP_ApplicationWorker_t* worker = &app->workerList[i];
        if(worker->ShutdownCallback)
        {
            worker->ShutdownCallback(app, worker->data);
        }
    }

    app->workerCount = 0;
    RGP_ListDelete(app->workerList);

    RGP_LOG_SUCCESS("App workers shutdowned");
}

void RGP_AppRun(RGP_Application_t* app)
{
    RGP_LOG_SUCCESS("Application started");
    while(app->isRunning)
    {
        RGP_WindowPoll();

        RGP_RendererClear(&app->rdrD);

        for(size_t i = 0; i < app->workerCount; ++i)
        {
            RGP_ApplicationWorker_t* worker = &app->workerList[i];
            if(worker->UpdateCallback)
            {
                worker->UpdateCallback(app, worker->data);
            }
        }

        RGP_GuiBegin();
        RGP_GuiEnd(&app->window);

        RGP_WindowSwapBuffers(&app->window);
    }
}

void RGP_AppStop(RGP_Application_t* app)
{
    RGP_LOG_SUCCESS("Application stopped");
    app->isRunning = 0;
}
