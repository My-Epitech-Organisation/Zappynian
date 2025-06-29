/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Food timer implementation
*/

#include "../include/food_timer.h"
#include "../include/player.h"
#include "../include/server.h"
#include <errno.h>

int food_timer_init(food_timer_t *ft)
{
    if (!ft) {
        errno = EINVAL;
        return -1;
    }
    ft->ticks_since_last_consumption = 0;
    ft->initialized = true;
    return 0;
}

void food_timer_destroy(food_timer_t *ft)
{
    if (!ft)
        return;
    ft->initialized = false;
    ft->ticks_since_last_consumption = 0;
}

void food_timer_tick(food_timer_t *ft)
{
    if (!ft || !ft->initialized)
        return;
    ft->ticks_since_last_consumption++;
}

bool food_timer_should_consume(food_timer_t *ft)
{
    if (!ft || !ft->initialized)
        return false;
    return ft->ticks_since_last_consumption >= FOOD_CONSUMPTION_INTERVAL;
}

void food_timer_reset(food_timer_t *ft)
{
    if (!ft || !ft->initialized)
        return;
    ft->ticks_since_last_consumption = 0;
}
