/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Time utility functions
*/

#ifndef TIME_UTILS_H
    #define TIME_UTILS_H

    #include <stdint.h>

/* Time utility functions */
uint64_t get_current_time_us(void);
uint64_t calculate_tick_interval(int frequency);

#endif /* TIME_UTILS_H */
