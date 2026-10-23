#pragma once
#include "Core.h"

typedef enum
{
    RPG_WINDOW_EVENT_TYPE_CLOSE,
    RPG_WINDOW_EVENT_TYPE_LEN
} RPG_WindowEventType;

struct RGP_Window_s
{
    void* native;
    int32_t width, height;
    const char* name;
    RGP_Application_t* ctx;
    void (*EventCallback)(RGP_Application_t*, RPG_WindowEventType);
    struct nk_context* guiContext;
};

Bool RGP_WindowInitializeAPI();

Bool RGP_WindowCreate(
        RGP_Window_t* outWindow, RGP_Application_t* application,
        int32_t width, int32_t height, const char* name,
        void (*EventCallback)(RGP_Application_t*, RPG_WindowEventType)
        );
void RGP_WindowDestroy(RGP_Window_t* outWindow);

void RGP_WindowPoll();
void RGP_WindowSwapBuffers(RGP_Window_t* window);
