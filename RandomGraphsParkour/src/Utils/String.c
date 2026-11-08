#include "Utils/String.h"

void RGP_StringFormat(char* dst, size_t bufferSize, const char* format, ...)
{
    RGP_ASSERT(dst != NULL && format != NULL, "dst and format should not be NULL");

    va_list list;
    va_start(list, format);

    vsnprintf(dst, bufferSize, format, list);

    va_end(list);
}
