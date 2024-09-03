/*  Arquivo contendo a macro que calcula o tempo de execução.
    Funciona em Windows e em sistemas POSIX
*/

#ifndef _CLOCK_TIMER_H
#define _CLOCK_TIMER_H

#ifdef _WIN32 // Verifica se está compilando no Windows
#include <windows.h>

#define GET_TIME(now) { \
    LARGE_INTEGER time, freq; \
    QueryPerformanceFrequency(&freq); \
    QueryPerformanceCounter(&time); \
    now = (double)time.QuadPart / freq.QuadPart; \
}

#else // Para sistemas POSIX (Linux, macOS)
#include <time.h>
#define BILLION 1000000000L

#define GET_TIME(now) { \
    struct timespec time; \
    clock_gettime(CLOCK_MONOTONIC, &time); \
    now = time.tv_sec + time.tv_nsec / 1000000000.0; \
}

#endif
#endif