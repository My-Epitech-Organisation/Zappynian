/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Timing manager for game frequency control
*/

#ifndef TIMING_MANAGER_H
    #define TIMING_MANAGER_H

    #include <sys/time.h>
    #include <stdint.h>
    #include <stdbool.h>

typedef struct timing_manager_s {
    int frequency;
    uint64_t tick_interval_us;
    uint64_t last_tick_time;
    uint64_t next_tick_time;
    bool initialized;
} timing_manager_t;

/* Timing manager core functions */
int timing_manager_init(timing_manager_t *tm, int frequency);
void timing_manager_destroy(timing_manager_t *tm);
bool timing_manager_should_tick(timing_manager_t *tm);
void timing_manager_mark_tick(timing_manager_t *tm);
int timing_manager_get_poll_timeout(timing_manager_t *tm);

#endif /* TIMING_MANAGER_H */
