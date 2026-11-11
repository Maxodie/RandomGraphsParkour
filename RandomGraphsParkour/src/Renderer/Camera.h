#pragma once
#include "Core.h"

typedef struct RGP_2DCamera_s
{
    ILDA_matrix4x4 view;
    ILDA_matrix4x4 projection;
    float aspectRatio;
    float near;
    float far;
    float width;
    float height;
    float zoom;

    ILDA_vector3f position;
} RGP_2DCamera_t;

void RGP_CameraInit(RGP_2DCamera_t* cam, float near, float far, float width, float height);
void RGP_CameraShutdown(RGP_2DCamera_t* cam);

void RGP_CameraZoom(RGP_2DCamera_t* cam, float zoomValue);

void RGP_CameraUpdateView(RGP_2DCamera_t* cam);
void RGP_CameraUpdatePosition(RGP_2DCamera_t* cam, ILDA_vector3f* position);
void RGP_CameraMove(RGP_2DCamera_t* cam, ILDA_vector3f* position);
void RGP_CameraResizeBound(RGP_2DCamera_t* cam, float width, float height);
