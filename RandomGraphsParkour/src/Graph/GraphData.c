#include "Graph/GraphData.h"
#include "Core.h"

void RGP_GenerateGraph(RGP_EditorScene_t* scene, uint32_t pointAmount, Bool canHaveCircuit, Bool isWeigthed)
{
    RGP_ASSERT(scene != NULL, "scene must not be NULL");
    RGP_ASSERT(pointAmount > 0, "the point amount must be > 0");

    scene->arcs = RGP_ListInit(pointAmount, *scene->arcs);

    scene->pointCount = pointAmount;
    scene->graphPoints = RGP_ListInit(pointAmount, *scene->graphPoints);

    for(size_t i = 0; i < pointAmount; ++i)
    {

    }
}

