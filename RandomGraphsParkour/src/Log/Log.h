#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>

typedef enum {
    LOG_DEBUG,
    LOG_SUCCESS,
    LOG_WARNING,
    LOG_ERROR,
    LOG_LEVEL_LEN
} LOG_LEVEL;

void RGP_LoggerLog(LOG_LEVEL level, const char* fmt, ...);
void RGP_ContextLoggerLog(LOG_LEVEL level, const char* functionCtx, const char* fileCtx, uint32_t lineCtx, const char* fmt, ...);

#define RGP_LOG_DEBUG(...) RGP_LoggerLog(LOG_DEBUG, __VA_ARGS__)
#define RGP_LOG_SUCCESS(...) RGP_LoggerLog(LOG_SUCCESS, __VA_ARGS__)
#define RGP_LOG_WARNING(...) RGP_ContextLoggerLog(LOG_WARNING, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define RGP_LOG_ERROR(...) RGP_ContextLoggerLog(LOG_ERROR, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define RGP_LOG_ERROR_ASSERT(...) RGP_LoggerLog(LOG_ERROR, __VA_ARGS__)
