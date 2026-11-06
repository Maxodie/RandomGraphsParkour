#pragma once
#include "Application.h"

typedef struct RGP_GraphArc_s RGP_GraphArc_t;
typedef struct RGP_GraphPoint_s RGP_GraphPoint_t;

struct RGP_GraphPoint_s
{
    ILDA_vector3f position;
    ILDA_vector3f color;
    RGP_GraphArc_t* nexts;
    RGP_GraphArc_t* previous;
    int32_t value;
};

struct RGP_GraphArc_s
{
    RGP_GraphPoint_t* start;
    RGP_GraphPoint_t* end;
    int32_t value;
    Bool isDirected;
    Bool isWeigthed;
};

typedef struct RGP_EditorScene_t
{
    RGP_Application_t* app;
    RGP_GraphPoint_t* graphPoints;
    RGP_GraphArc_t* arcs;
    size_t pointCount;

    RGP_2DCamera_t cam;
    Bool isRunning;
    const char padding[7];
} RGP_EditorScene_t;

void RGP_GenerateGraph(RGP_EditorScene_t* scene, uint32_t pointAmount, Bool canHaveCircuit, Bool isWeigthed);
