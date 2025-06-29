/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Timing integration for server
*/

#ifndef TIMING_INTEGRATION_H
    #define TIMING_INTEGRATION_H

    #include "server.h"
    #include "food_timer.h"

/* Server timing integration */
int server_timing_init(server_t *server);
void server_timing_destroy(server_t *server);
void server_timing_tick(server_t *server);
int server_get_poll_timeout(server_t *server);
bool server_should_tick(server_t *server);

#endif /* TIMING_INTEGRATION_H */
