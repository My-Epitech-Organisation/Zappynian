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
