#pragma once
#include "Core.h"
#include "Graph/GraphData.h"

void RGP_EditorInit(RGP_EditorScene_t* scene);
void RGP_EditorShutdown(RGP_EditorScene_t* scene);

void RGP_EditorUpdate(RGP_EditorScene_t* scene);
void RGP_EditorUpdateGUI(RGP_EditorScene_t* scene);
