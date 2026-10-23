#pragma once
#include "Window/Window.h"

struct RGP_Application_s
{
    Bool isRunning;
    RGP_Window_t window;
};

void RGP_AppInit(RGP_Application_t* app);
void RGP_AppShutdown(RGP_Application_t* app);

void RGP_AppHandleEvent(RGP_Application_t* app, RPG_WindowEventType eventType);

void RGP_AppRun(RGP_Application_t* app);
void RGP_AppStop(RGP_Application_t* app);
