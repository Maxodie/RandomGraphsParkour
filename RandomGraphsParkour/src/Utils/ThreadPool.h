#pragma once
#include "Core.h"
#include "sys/sysinfo.h"

typedef struct RGP_ThreadWork_s RGP_ThreadWork_t;
typedef struct RGP_ThreadPool_s RGP_ThreadPool_t;

RGP_ThreadPool_t* RGP_ThreadCreate(size_t amount);
void RGP_ThreadDestroy(RGP_ThreadPool_t* pool);

void RGP_ThreadWait(RGP_ThreadPool_t* pool);
Bool RGP_ThreadAddWork(RGP_ThreadPool_t* pool, RGP_RoutineFunc_t routine, void* arg);

static inline int RGP_ThreadMaxAmount()
{
    return get_nprocs();
}
