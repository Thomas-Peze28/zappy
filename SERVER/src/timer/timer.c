/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** timer
*/

#include <sys/time.h>
#include <stddef.h>

long long get_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec * 1000LL
        + (long long)tv.tv_usec / 1000LL);
}
