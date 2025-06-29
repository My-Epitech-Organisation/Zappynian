/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Food timer management
*/

#ifndef FOOD_TIMER_H
    #define FOOD_TIMER_H

    #include <stdbool.h>

    #define FOOD_CONSUMPTION_INTERVAL 126

typedef struct player_s player_t;
typedef struct server_s server_t;

typedef struct food_timer_s {
    int ticks_since_last_consumption;
    bool initialized;
} food_timer_t;

/* Food timer functions */
int food_timer_init(food_timer_t *ft);
void food_timer_destroy(food_timer_t *ft);
void food_timer_tick(food_timer_t *ft);
bool food_timer_should_consume(food_timer_t *ft);
void food_timer_reset(food_timer_t *ft);

/* Player food management */

#endif /* FOOD_TIMER_H */
