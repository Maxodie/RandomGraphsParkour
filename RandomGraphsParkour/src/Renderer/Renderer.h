#pragma once
#include  "Core.h"
#include "Renderer/Camera.h"
#include "Renderer/Text.h"
#define RGP_TEXTURE_MAX_COUNT 32

//GLuint is an unisgned int
struct RGP_RendererData_s
{
    RGP_Window_t* window;

    unsigned int VAO;

    unsigned int vertexbufferID;
    unsigned int indexbufferID;

    unsigned int programID;
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;

    RGP_Vertex_t* vertexBuffer;
    uint32_t* indexBuffer;
    size_t indexBufferOffset;
    uint32_t vertexBufferOffset;
    uint32_t currentIndexOffset;

    RGP_FontLoader_t* fontLoader;
    RGP_Font_t* defaultFont;

    uint32_t textures[RGP_TEXTURE_MAX_COUNT];
    uint32_t textureOffset;

    size_t drawCallCountPerUpdate;

    Bool cachedShowPoints;
};

void RGP_RendererInit(RGP_RendererData_t* rdrD, RGP_Window_t* window);
void RGP_RendererShutdown(RGP_RendererData_t* rdrD);

void RGP_RendererDrawPoint(RGP_RendererData_t* rdrD, const ILDA_vector3f* position, const ILDA_vector3f* color, float size);
void RGP_RendererDrawLine(RGP_RendererData_t* rdrD, const ILDA_vector3f* position1, const ILDA_vector3f* position2, const ILDA_vector3f* color, float width, Bool isDirected);
void RGP_RendererDrawText(RGP_RendererData_t* rdrD, const ILDA_vector3f* position, const ILDA_vector3f* color, const char* text, float size);

uint32_t RGP_RendererAddDrawTexture(RGP_RendererData_t* rdrD, uint32_t texID);
void RGP_RendererClear(RGP_RendererData_t* rdrD);
void RGP_RendererDrawCall(RGP_RendererData_t* rdrD, Bool showPoints);

void RGP_RendererBegin(RGP_RendererData_t* rdrD, RGP_2DCamera_t* cam);
void RGP_RendererEnd(RGP_RendererData_t* rdrD, Bool showPoints);
void RGP_RendererReset(RGP_RendererData_t* rdrD);
