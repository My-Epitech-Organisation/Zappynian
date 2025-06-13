/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Resource spawning functions
*/

#include <time.h>
#include <stdbool.h>
#include "../includes/world.h"
#include "../includes/server.h"
#include "../includes/resource.h"

void respawn_resources(void)
{
    static bool seeded = false;
    server_t *server = get_server_instance();

    if (!server || !server->map)
        return;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    resource_respawn(server->map);
}
