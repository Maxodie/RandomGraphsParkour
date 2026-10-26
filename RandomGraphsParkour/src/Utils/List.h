#pragma once
#include "Core.h"

void* RGP_ListInit(size_t reserve, size_t typeSize);
void RGP_ListAdd(void** list, size_t sizeToAdd);
void RGP_ListDelete(void* list);
