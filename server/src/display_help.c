/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** display_help
*/

#include "../include/server.h"

int display_help(void)
{
    printf("Usage: ./zappy_server -p port -x width -y height");
    printf(" -n name1 name2 ... -c clientsNb -f freq\n");
    return 1;
}
