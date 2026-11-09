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

    size_t drawCallCountPerUpdate;

    Bool cachedShowPoints;
};

struct RGP_PointData_s
{
    ILDA_vector3f* position;
    float size;
};

struct RGP_LineData_s
{
    ILDA_vector3f* position1;
    ILDA_vector3f* position2;
    float width;
};

void RGP_RendererInit(RGP_RendererData_t* rdrD, RGP_Window_t* window);
void RGP_RendererShutdown(RGP_RendererData_t* rdrD);

void RGP_RendererDrawPoint(RGP_RendererData_t* rdrD, const RGP_PointData_t* pointData, const ILDA_vector3f* color);
void RGP_RendererDrawLine(RGP_RendererData_t* rdrD, const RGP_LineData_t* lineData, const ILDA_vector3f* color, Bool isDirected);

//font can be NULL (the default font will be used)
void RGP_RendererDrawPointCenteredText(RGP_RendererData_t* rdrD, const RGP_Font_t* font, const RGP_PointData_t* pointData, const ILDA_vector3f* color, const char* text, float textSize);
//font can be NULL (the default font will be used)
void RGP_RendererDrawLineCenteredText(RGP_RendererData_t* rdrD, const RGP_Font_t* font, const RGP_LineData_t* lineData, const ILDA_vector3f* color, const char* text, float textSize);
//font can be NULL (the default font will be used)
void RGP_RendererDrawText(RGP_RendererData_t* rdrD, const RGP_Font_t* font, const ILDA_vector3f* position, const ILDA_vector3f* color, const char* text, float size);

void RGP_RendererClear(RGP_RendererData_t* rdrD);
void RGP_RendererDrawCall(RGP_RendererData_t* rdrD);

void RGP_RendererBegin(RGP_RendererData_t* rdrD, RGP_2DCamera_t* cam);
void RGP_RendererEnd(RGP_RendererData_t* rdrD, Bool showPoints);
void RGP_RendererReset(RGP_RendererData_t* rdrD);
