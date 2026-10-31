#pragma once
#include "Application.h"

typedef struct RGP_GraphPoint_s
{
    ILDA_vector3f position;
    ILDA_vector3f color;
} RGP_GraphPoint_t;

typedef struct RGP_EditorScene_t
{
    RGP_Application_t* app;
    RGP_GraphPoint_t* graphPoints;
    size_t pointCount;

    Bool isRunning;
} RGP_EditorScene_t;
