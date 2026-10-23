#include "Log/Log.h"

#ifdef __EMSCRIPTEN
#define NO_ANSI
#endif

char* type[LOG_LEVEL_LEN] = {
    "DEBUG",
    "SUCCESS",
    "WARNING",
    "ERROR"
};

char* colors[LOG_LEVEL_LEN] = {
    "\x1b[34m",
    "\x1b[32m",
    "\x1b[1;33m",
    "\x1b[31m"
};

struct tm* m_time;
void RGP_LoggerGetTime()
{
    time_t current_time;
    time(&current_time);
    m_time = localtime(&current_time);
}

void RGP_PrefixLog(LOG_LEVEL level)
{
    RGP_LoggerGetTime();

#ifndef NO_ANSI
    printf("%s", colors[level]);
#endif

    printf("[%d:%d:%d][%s] ",
        m_time->tm_hour,
        m_time->tm_min,
        m_time->tm_sec,
        type[level]
    );
}

void RGP_SuffixLog()
{
    printf("\n%s", colors[LOG_DEBUG]);
}

void RGP_LoggerLog(LOG_LEVEL level, const char* fmt, ...) {

    va_list args;
    va_start(args, fmt);

    RGP_PrefixLog(level);

    vfprintf(stdout, fmt, args);

    RGP_SuffixLog();

    va_end(args);
}



void RGP_ContextLoggerLog(LOG_LEVEL level, const char* functionCtx, const char* fileCtx, uint32_t lineCtx, const char* fmt, ...)
{

    va_list args;
    va_start(args, fmt);

    RGP_PrefixLog(level);

    printf("%s() from %s, line %d : ", functionCtx, fileCtx, lineCtx);

    vfprintf(stdout, fmt, args);

    RGP_SuffixLog();

    va_end(args);
}
