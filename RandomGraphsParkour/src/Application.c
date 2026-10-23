#include "Application.h"
#include "Log/Log.h"
#include "GUILayer/GUILayer.h"

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

    RGP_GuiInit(&app->window);

    app->isRunning = 1;
    RGP_LOG_SUCCESS("Application started");
}

void RGP_AppShutdown(RGP_Application_t* app)
{
    RGP_GuiShutdown();

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

void RGP_AppRun(RGP_Application_t* app)
{
    while(app->isRunning)
    {
        RGP_WindowPoll();

        RGP_GuiBegin();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.18f, 0.18f, 0.18f, 1.f);

        RGP_GuiEnd(&app->window);

        RGP_WindowSwapBuffers(&app->window);
    }
}

void RGP_AppStop(RGP_Application_t* app)
{
    RGP_LOG_SUCCESS("Application stopped");
    app->isRunning = 0;
}
