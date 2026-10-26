#include "Utils/List.h"

typedef struct RGP_ListData_s
{
    size_t dataSize;
    size_t size;
} RGP_ListData_t;

void* RGP_ListInit(size_t reserve, size_t typeSize)
{
    RGP_ASSERT(reserve != 0 && typeSize != 0, "default reserve size and type size should not be 0");
    void* data = malloc(sizeof(RGP_ListData_t) + reserve * typeSize);
    if(!data)
    {
        RGP_LOG_ERROR("allocation returned NULL");
        return data;
    }

    return data + sizeof(RGP_ListData_t);
}

void RGP_ListAdd(void** list, size_t sizeToAdd)
{
    RGP_ASSERT(list != NULL && sizeToAdd != 0, "default reserve size and list should not be NULL");
    RGP_ASSERT(*list != NULL, "list should not be NULL");
    RGP_ListData_t* listData = *list - sizeof(RGP_ListData_t);
    void* data = realloc(listData, listData->size * listData->dataSize + sizeof(RGP_ListData_t) + sizeToAdd * listData->dataSize);
    if(!data)
    {
        RGP_LOG_ERROR("failed to realloc data");
        return;
    }

    *list = data + sizeof(RGP_ListData_t);
}

void RGP_ListDelete(void* list)
{
    RGP_ASSERT(list != NULL, "list should not be NULL");
    free(list - sizeof(RGP_ListData_t));
}
