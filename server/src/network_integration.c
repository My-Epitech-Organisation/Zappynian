/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Network integration adapters
*/

#include "../includes/server.h"

int init_network_integration(void)
{
    return zn_init();
}

void cleanup_network_integration(void)
{
    zn_cleanup();
}

zn_socket_t setup_zappy_server_socket(int port)
{
    return zn_server_listen(port);
}
