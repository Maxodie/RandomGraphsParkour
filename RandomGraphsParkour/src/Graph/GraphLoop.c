#include "Graph/GraphLoop.h"
#include "Application.h"
#include "Renderer/Renderer.h"

void RGP_EditorInit(RGP_Application_t* app, RGP_EditorScene_t* scene)
{
    RGP_LOG_SUCCESS("Editor initialized");
}

void RGP_EditorShutdown(RGP_Application_t* app, RGP_EditorScene_t* scene)
{
    RGP_LOG_SUCCESS("Editor shutdowned");
}

void RGP_EditorUpdate(RGP_Application_t* app, RGP_EditorScene_t* scene)
{
    RGP_RendererBegin(&app->rdrD);

    ILDA_vector3f color1 = {1.f, 0.0f, 0.5f};
    ILDA_vector3f color2 = {0.0f, 0.1f, 0.5f};

    ILDA_vector3f location1 = {0.5f, 0.5f, 0.5f};
    RGP_RendererDrawPoint(&app->rdrD, &location1, &color1);
    ILDA_vector3f location2 = {-0.5f, -0.5f, -0.5f};
    RGP_RendererDrawPoint(&app->rdrD, &location2, &color1);
    ILDA_vector3f location3 = {0.f, 0.f, 0.f};
    RGP_RendererDrawPoint(&app->rdrD, &location3, &color2);

    RGP_RendererEnd(&app->rdrD);
}
