/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Time utility functions implementation
*/

#include "../include/time_utils.h"
#include <sys/time.h>
#include <stddef.h>

uint64_t get_current_time_us(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        return 0;
    return (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
}

uint64_t calculate_tick_interval(int frequency)
{
    if (frequency <= 0)
        return 0;
    return 1000000 / frequency;
}
