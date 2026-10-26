#pragma once
#include "Window/Window.h"
#include <Renderer/Renderer.h>

struct RGP_Application_s
{
    RGP_Window_t window;
    RGP_RendererData_t rdrD;
    Bool isRunning;
};

void RGP_AppInit(RGP_Application_t* app);
void RGP_AppShutdown(RGP_Application_t* app);

void RGP_AppHandleEvent(RGP_Application_t* app, RPG_WindowEventType eventType);

void RGP_AppRun(RGP_Application_t* app);
void RGP_AppStop(RGP_Application_t* app);
