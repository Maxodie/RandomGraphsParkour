#pragma once

#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <limits.h>
#include <float.h>

#include <ILDA_vector/ILDA_vector.h>
#include <ILDA_matrix/ILDA_matrix.h>
#include <ILDA_utils/ILDA_utils.h>

typedef uint8_t Bool;
typedef struct RGP_Application_s RGP_Application_t;
typedef struct RGP_Window_s RGP_Window_t;
typedef struct RGP_RendererData_s RGP_RendererData_t;
typedef struct RGP_Vertex_s RGP_Vertex_t;
typedef struct RGP_ListData_s RGP_ListData_t;

typedef struct RGP_PointData_s RGP_PointData_t;
typedef struct RGP_LineData_s RGP_LineData_t;

typedef void (*RGP_RoutineFunc_t)(void*);

#ifdef RGP_ASSERT_CORE
#define RGP_ASSERT(x, ...)\
    do\
    {\
        if(!(x))\
        {RGP_LOG_ERROR_ASSERT(__VA_ARGS__); assert(0); }\
    }\
    while(0)\

#else
#define RGP_ASSERT(x, msg)
#endif

#include "Log/Log.h"
#include "Utils/List.h"
#include "Utils/String.h"
#include "Utils/ThreadPool.h"
