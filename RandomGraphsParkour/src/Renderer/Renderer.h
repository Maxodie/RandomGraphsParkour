#pragma once
#include  "Core.h"

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
    size_t vertexBufferOffset;
    uint32_t* indexBuffer;
    size_t indexBufferOffset;
    size_t currentIndexOffset;
};

void RGP_RendererInit(RGP_RendererData_t* rdrD, RGP_Window_t* window);
void RGP_RendererShutdown(RGP_RendererData_t* rdrD);

void RGP_RendererClear(RGP_RendererData_t* rdrD);
void RGP_RendererDrawPoint(RGP_RendererData_t* rdrD, const ILDA_vector3f* position, const ILDA_vector3f* color, float size);
void RGP_RendererDrawLine(RGP_RendererData_t* rdrD, const ILDA_vector3f* position1, const ILDA_vector3f* position2, const ILDA_vector3f* color, float width);
void RGP_RendererDrawCall(RGP_RendererData_t* rdrD);

void RGP_RendererBegin(RGP_RendererData_t* rdrD);
void RGP_RendererEnd(RGP_RendererData_t* rdrD);
void RGP_RendererFlush(RGP_RendererData_t* rdrD);
