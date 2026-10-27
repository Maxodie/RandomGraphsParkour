#include "Renderer/Renderer.h"
#include "Core.h"
#include "Renderer/Vertex.h"

#include <glad/glad.h>

static const char* vertexShaderData =
"#version 330 core\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"out vec3 outColor;\n"
"void main(){\n"
"    gl_Position = vec4(pos, 1.f); \n"
"    outColor = color; \n"
 "}";

static const char* fragmentShaderData =
"#version 330 core\n"
"out vec4 fragColor;\n"
"in vec3 outColor;\n"
"void main(){\n"
"    fragColor = vec4(outColor, 1.f);\n"
"}";

static const uint32_t g_triangleVertexAmount = 3;
static const size_t g_trianglebufferMaxSize = g_triangleVertexAmount * 512;

static const ILDA_vector3f g_triangleVertexBufferData[] = {
    { -0.5f, -0.5f, 0.0f },
    {  0.5f, -0.5f, 0.0f },
    { 0.0f,  0.5f, 0.0f }
};

void RGP_LoadShaders(RGP_RendererData_t* rdrD, const char* vertexData, const char* fragmentData)
{
    RGP_ASSERT(rdrD != NULL,  "the given renderer data should not be NULL");
    RGP_ASSERT(vertexData != NULL,  "the given vertex data should not be NULL");
    RGP_ASSERT(fragmentData != NULL,  "the given fragment data should not be NULL");

    rdrD->vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    rdrD->fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    rdrD->programID = glCreateProgram();

    GLint result = GL_FALSE;
    int infoLength;
    char* infoLogs;

    RGP_LOG_DEBUG("Compiling vertex shader...");
    glShaderSource(rdrD->vertexShaderID, 1, &vertexData, NULL);
    glCompileShader(rdrD->vertexShaderID);

    glGetShaderiv(rdrD->vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(rdrD->vertexShaderID, GL_INFO_LOG_LENGTH, &infoLength);

    if(result != GL_FALSE)
    {
        RGP_LOG_SUCCESS("vertex shader compiled");
    }
    else
    {
        infoLogs = malloc(infoLength * sizeof(char));
        glGetShaderInfoLog(rdrD->vertexShaderID, infoLength, &infoLength, infoLogs);
        RGP_LOG_WARNING("failed to compile vertex shader [%d] %s", infoLength, infoLogs);
        free(infoLogs);
        return;
    }

    RGP_LOG_DEBUG("Compiling fragment shader...");
    glShaderSource(rdrD->fragmentShaderID, 1, &fragmentData, NULL);
    glCompileShader(rdrD->fragmentShaderID);

    glGetShaderiv(rdrD->fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(rdrD->fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLength);
    if(result != GL_FALSE)
    {
        RGP_LOG_SUCCESS("fragment shader compiled");
    }
    else
    {
        infoLogs = malloc(infoLength * sizeof(char));
        glGetShaderInfoLog(rdrD->fragmentShaderID, infoLength, NULL, infoLogs);
        RGP_LOG_WARNING("failed to compile fragment shader [%d] %s", infoLength, infoLogs);
        free(infoLogs);
        return;
    }

    RGP_LOG_DEBUG("start combining shaders...");
    glAttachShader(rdrD->programID, rdrD->vertexShaderID);
    glAttachShader(rdrD->programID, rdrD->fragmentShaderID);
    glLinkProgram(rdrD->programID);

    glGetProgramiv(rdrD->programID, GL_LINK_STATUS, &result);
    glGetProgramiv(rdrD->fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLength);
    if(result != GL_FALSE)
    {
        RGP_LOG_SUCCESS("shader program linked");
    }
    else
    {
        infoLogs = malloc(infoLength * sizeof(char));
        glGetProgramInfoLog(rdrD->fragmentShaderID, infoLength, NULL, infoLogs);
        RGP_LOG_WARNING("failed to link the shader program [%d] %s", infoLength, infoLogs);
        free(infoLogs);
        return;
    }
}

void RGP_BindArrayBufferElement(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, uint32_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(
            index,
            size,
            type,
            normalized,
            stride * sizeof(float),
            (void*)(offset * sizeof(float))
            );
}

void RGP_RendererInit(RGP_RendererData_t* rdrD)
{
    RGP_ASSERT(rdrD != NULL,  "the given renderer data should not be NULL");
    rdrD->vertexBuffer = RGP_ListInit(g_trianglebufferMaxSize, RGP_Vertex_t);
    rdrD->indexBuffer = RGP_ListInit(g_trianglebufferMaxSize, uint32_t);

    glGenBuffers(1, &rdrD->vertexbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, rdrD->vertexbufferID);
    glBufferData(GL_ARRAY_BUFFER, g_trianglebufferMaxSize * sizeof(RGP_Vertex_t), (const void*)rdrD->vertexBuffer, GL_STATIC_DRAW);

    glGenBuffers(1, &rdrD->indexbufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rdrD->indexbufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_trianglebufferMaxSize * sizeof(uint32_t), (const void*)rdrD->indexBuffer, GL_STATIC_DRAW);

    glGenVertexArrays(1, &rdrD->VAO);
    glBindVertexArray(rdrD->VAO);
    RGP_BindArrayBufferElement(0, 3, GL_FLOAT, GL_FALSE, 6, 0);//position
    RGP_BindArrayBufferElement(1, 3, GL_FLOAT, GL_FALSE, 6, 3);//color

    RGP_LoadShaders(rdrD, vertexShaderData, fragmentShaderData);

    RGP_LOG_SUCCESS("Renderer initialized");
}

void RGP_RendererShutdown(RGP_RendererData_t* rdrD)
{
    RGP_ASSERT(rdrD != NULL,  "the given renderer data should not be NULL");

    glDeleteProgram(rdrD->programID);
    glDeleteShader(rdrD->fragmentShaderID);
    glDeleteShader(rdrD->vertexShaderID);
    RGP_LOG_SUCCESS("shaders and program destroyed");

    glDeleteBuffers(1, &rdrD->vertexbufferID);
    glDeleteBuffers(1, &rdrD->indexbufferID);

    RGP_ListDelete(rdrD->vertexBuffer);
    RGP_ListDelete(rdrD->indexBuffer);
    RGP_LOG_SUCCESS("Renderer shutdowned");
}

void RGP_RendererClear(RGP_RendererData_t* rdrD)
{
    RGP_ASSERT(rdrD != NULL,  "the given renderer data should not be NULL");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.18f, 0.18f, 0.38f, 1.f);

    glViewport(0,0,(GLsizei)840,(GLsizei)600);
}

void RGP_RendererDrawPoint(RGP_RendererData_t* rdrD, const ILDA_vector3f* position, const ILDA_vector3f* color)
{
    if(rdrD->vertexBufferOffset + g_triangleVertexAmount >= g_trianglebufferMaxSize)
    {
        RGP_RendererDrawCall(rdrD);
    }

    ILDA_vector3f finalPos;
    for(int i = 0; i < g_triangleVertexAmount; ++i)
    {
        ILDA_vector3f_copy(&finalPos, &g_triangleVertexBufferData[i]);
        ILDA_vector3f_add(&finalPos, position);
        rdrD->vertexBuffer[rdrD->vertexBufferOffset].position = finalPos;
        rdrD->vertexBuffer[rdrD->vertexBufferOffset].color = *color;
        ++rdrD->vertexBufferOffset;
    }

    size_t index = rdrD->indexBufferOffset;
    rdrD->indexBuffer[index] = rdrD->indexBufferOffset;
    rdrD->indexBuffer[++index] = rdrD->indexBufferOffset + 1;
    rdrD->indexBuffer[++index] = rdrD->indexBufferOffset + 2;

    rdrD->indexBufferOffset += 3;
}

void RGP_RendererDrawCall(RGP_RendererData_t* rdrD)
{
    //buffers
    glBindBuffer(GL_ARRAY_BUFFER, rdrD->vertexbufferID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, rdrD->indexBufferOffset * sizeof(uint32_t), (const void*)rdrD->indexBuffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rdrD->indexbufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, rdrD->vertexBufferOffset * sizeof(RGP_Vertex_t), (const void*)rdrD->vertexBuffer);

    //shader
    glUseProgram(rdrD->programID);

    //draw call
    glBindVertexArray(rdrD->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rdrD->indexbufferID);
    glDrawElements(GL_TRIANGLES, rdrD->indexBufferOffset, GL_UNSIGNED_INT, (const void*)0);

    //clear state
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    RGP_RendererFlush(rdrD);
}

void RGP_RendererBegin(RGP_RendererData_t* rdrD)
{
}

void RGP_RendererEnd(RGP_RendererData_t* rdrD)
{
    RGP_RendererDrawCall(rdrD);
}

void RGP_RendererFlush(RGP_RendererData_t* rdrD)
{
    rdrD->vertexBufferOffset = 0;
    rdrD->indexBufferOffset = 0;
}
