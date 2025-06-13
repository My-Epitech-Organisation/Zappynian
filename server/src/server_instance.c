/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** server_instance
*/

#include "../includes/server.h"

static server_t *g_server_instance = NULL;

server_t *get_server_instance(void)
{
    return g_server_instance;
}

void set_server_instance(server_t *server)
{
    g_server_instance = server;
}
