#pragma once

#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <memory.h>
#include <time.h>
#include <malloc.h>

#include "Log/Log.h"
#include "Utils/List.h"
#include <ILDA_vector/ILDA_vector.h>

typedef char Bool;
typedef struct RGP_Application_s RGP_Application_t;
typedef struct RGP_Window_s RGP_Window_t;
typedef struct RGP_RendererData_s RGP_RendererData_t;
typedef struct RGP_Vertex_s RGP_Vertex_t;

#ifdef RGP_ASSERT_CORE
#define RGP_ASSERT(x, msg)\
    do\
    {\
        if(!(x))\
        {RGP_LOG_ERROR_ASSERT(msg); assert(0); }\
    }\
    while(0)\

#else
#define RGP_ASSERT(x, msg)
#endif
