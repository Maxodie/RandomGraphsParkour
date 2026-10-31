#include "Utils/List.h"
#include <ILDA_utils/ILDA_utils.h>

void* RGP_ListInit_imp(size_t reserve, size_t typeSize)
{
    RGP_ASSERT(reserve != 0 && typeSize != 0, "default reserve size and type size should not be 0");
    void* data = malloc(sizeof(RGP_ListData_t) + reserve * typeSize);
    if(!data)
    {
        RGP_LOG_ERROR("allocation returned NULL");
        return data;
    }

    RGP_ListData_t* listData = data;
    listData->size = reserve;
    listData->dataSize = typeSize;
    listData->count = 0;

    return data + sizeof(RGP_ListData_t);
}

void RGP_ListDelete_imp(void* list)
{
    RGP_ASSERT(list != NULL, "list should not be NULL");
    free(list - sizeof(RGP_ListData_t));
}

void RGP_ListAddUb_imp(void** list)
{
    RGP_ASSERT(list != NULL && *list, "given list should not be NULL");
    RGP_ListData_t* listData = *list - sizeof(RGP_ListData_t);

    if(listData->count >= listData->size)
    {
        void* newData = realloc(listData, sizeof(RGP_ListData_t) + listData->size * 2 * listData->dataSize); //reallocate x2 the size of the list
        if(!newData)
        {
            RGP_LOG_ERROR("failed to realloc data");
            return;
        }
        listData = newData;

        listData->size *= 2;
        *list = (void*)((uintptr_t)listData + sizeof(RGP_ListData_t));
    }

    ++listData->count;
}

void RGP_ListAddCpy_imp(void** list, const void* value)
{
    RGP_ListAddUb_imp(list);

    RGP_ListData_t* listData = *list - sizeof(RGP_ListData_t);
    memcpy(*list + (listData->count - 1) * listData->dataSize, value, listData->dataSize);
}

void RGP_ListRemoveLast_imp(void* list)
{
    RGP_ASSERT(list != NULL, "given list should not be NULL");

    RGP_ListData_t* listData = list - sizeof(RGP_ListData_t);
    if(listData->count > 0)
    {
        --listData->count;
    }
}
