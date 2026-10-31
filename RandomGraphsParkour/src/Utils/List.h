#pragma once
#include "Core.h"

struct RGP_ListData_s
{
    size_t dataSize;
    size_t size;
    size_t count;
};

#define RGP_ListInit(reserve, type) RGP_ListInit_imp(reserve, sizeof(type))
#define RGP_ListDelete(list) RGP_ListDelete_imp((void*)list)

#define RGP_ListAddCpy(list, value) RGP_ListAddCpy_imp((void**)&list, value)
#define RGP_ListAddUb(list) RGP_ListAddUb_imp((void**)&list)

#define RGP_ListRemoveLast(list) RGP_ListRemoveLast_imp((void*)list)

#define RGP_ListGetCount(list) ((RGP_ListData_t*)((uintptr_t)list - sizeof(RGP_ListData_t)))->count

void* RGP_ListInit_imp(size_t reserve, size_t typeSize);
void RGP_ListDelete_imp(void* list);

void RGP_ListAddUb_imp(void** list);
void RGP_ListAddCpy_imp(void** list, const void* value);

//must not shrink array
void RGP_ListRemoveLast_imp(void* list);
