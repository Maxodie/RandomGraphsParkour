#include "Editor/GraphData.h"
#include "Core.h"

void RGP_GraphGeneratorProcess(RGP_EditorScene_t* scene)
{
    if(scene->graphPoints != NULL)
    {
        RGP_GraphDestroy(scene);
    }
    RGP_GraphGenerate(scene, 10, 1, 1);
}

void RGP_GraphGenerate(RGP_EditorScene_t* scene, uint32_t pointAmount, Bool canHaveCircuit, Bool isWeigthed)
{
    RGP_ASSERT(scene != NULL, "scene must not be NULL");
    RGP_ASSERT(pointAmount > 0, "the point amount must be > 0");

    scene->arcs = RGP_ListInit(pointAmount, *scene->arcs);

    scene->graphPoints = RGP_ListInit(pointAmount, *scene->graphPoints);
    scene->arcs = RGP_ListInit(pointAmount - 1, *scene->arcs);

    size_t colSize = pointAmount / 2;
    for(size_t i = 0; i < pointAmount; ++i)
    {
        float x = i % colSize;
        float y = floor(i / (float)colSize);

        ++scene->pointCount;
        RGP_ListAddUb(scene->graphPoints);
        scene->graphPoints[i].position = (ILDA_vector3f){ .x = x, .y = -y, .z = 0.0f};
        scene->graphPoints[i].color = (ILDA_vector3f){ .x = 1, .y = 0.3f, .z = 0.3f};
        scene->graphPoints[i].value = i;

        scene->graphPoints[i].nexts = RGP_ListInit(2, *scene->graphPoints[i].nexts);
    }

    for(size_t i = 1; i < pointAmount; ++i)
    {
        ++scene->arcCount;
        RGP_ListAddUb(scene->arcs);
        int32_t value = ILDA_randomi(-20, 20);
        scene->arcs[i - 1].start = &scene->graphPoints[i - 1];
        scene->arcs[i - 1].end = &scene->graphPoints[i];
        scene->arcs[i - 1].value = value;
        scene->arcs[i - 1].color = (ILDA_vector3f){ .x = 0.3f, 0.3f, 1.f };
        scene->arcs[i - 1].isDirected = 1;
        scene->arcs[i - 1].isWeigthed = isWeigthed;

        RGP_ListAddCpy(scene->graphPoints[i].nexts, &scene->arcs[i - 1]);
    }
}

void RGP_GraphDestroy(RGP_EditorScene_t* scene)
{
    RGP_ASSERT(scene != NULL, "scene must not be NULL");

    scene->arcCount = 0;
    scene->pointCount = 0;

    for(size_t i = 1; i < scene->pointCount; ++i)
    {
        RGP_ListDelete(scene->graphPoints[i].nexts);
    }

    RGP_ListDelete(scene->arcs);
    RGP_ListDelete(scene->graphPoints);
}
