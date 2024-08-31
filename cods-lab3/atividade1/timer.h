#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#ifdef _WIN32
#include <windows.h>

#define GET_TIME(now) { \
    LARGE_INTEGER time, freq; \
    QueryPerformanceFrequency(&freq); \
    QueryPerformanceCounter(&time); \
    now = (double)time.QuadPart / freq.QuadPart; \
}
#endif
#endif