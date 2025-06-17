/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Network integration adapters
*/

#include "../includes/server.h"

static int init_zappy_net_library(void)
{
    if (zn_init() != 0) {
        return -1;
    }
    return 0;
}

static void cleanup_zappy_net_library(void)
{
    zn_cleanup();
}

static zn_socket_t create_server_socket(int port)
{
    return zn_server_listen(port);
}

int init_network_integration(void)
{
    return init_zappy_net_library();
}

void cleanup_network_integration(void)
{
    cleanup_zappy_net_library();
}

zn_socket_t setup_zappy_server_socket(int port)
{
    return create_server_socket(port);
}
