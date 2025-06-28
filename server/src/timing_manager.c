/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Timing manager implementation
*/

#include "../include/timing_manager.h"
#include <stdlib.h>
#include <errno.h>

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

int timing_manager_init(timing_manager_t *tm, int frequency)
{
    if (!tm || frequency <= 0) {
        errno = EINVAL;
        return -1;
    }
    tm->frequency = frequency;
    tm->tick_interval_us = calculate_tick_interval(frequency);
    tm->last_tick_time = get_current_time_us();
    tm->next_tick_time = tm->last_tick_time + tm->tick_interval_us;
    tm->initialized = true;
    return 0;
}

void timing_manager_destroy(timing_manager_t *tm)
{
    if (!tm)
        return;
    tm->initialized = false;
    tm->frequency = 0;
    tm->tick_interval_us = 0;
    tm->last_tick_time = 0;
    tm->next_tick_time = 0;
}

bool timing_manager_should_tick(timing_manager_t *tm)
{
    uint64_t current_time;

    if (!tm || !tm->initialized)
        return false;
    current_time = get_current_time_us();
    return current_time >= tm->next_tick_time;
}

void timing_manager_mark_tick(timing_manager_t *tm)
{
    if (!tm || !tm->initialized)
        return;
    tm->last_tick_time = get_current_time_us();
    tm->next_tick_time = tm->last_tick_time + tm->tick_interval_us;
}

int timing_manager_get_poll_timeout(timing_manager_t *tm)
{
    uint64_t current_time;
    uint64_t time_until_tick;

    if (!tm || !tm->initialized)
        return 100;
    current_time = get_current_time_us();
    if (current_time >= tm->next_tick_time)
        return 0;
    time_until_tick = tm->next_tick_time - current_time;
    return (int)(time_until_tick / 1000);
}
