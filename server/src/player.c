/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** player
*/

#include "../include/player.h"
#include "../include/server.h"
#include "../include/commands.h"
#include "../include/egg.h"

static void init_player_resources(player_t *player)
{
    int i;

    for (i = 0; i < RESOURCE_COUNT; i++)
        player->resources[i] = 0;
}

static void init_player_commands(player_t *player)
{
    int i;

    player->command_count = 0;
    player->current_command_line = NULL;
    for (i = 0; i < MAX_PLAYER_COMMANDS; i++) {
        player->commands[i] = NULL;
        player->command_timers[i] = 0;
    }
}

static void set_player_defaults(player_t *player, int id, int x, int y)
{
    player->id = id;
    player->x = x;
    player->y = y;
    player->level = 1;
    player->orientation = 0;
    player->food = 10;
    player->dead = false;
    player->in_elevation = false;
    player->slot_id = -1;
}

player_t *create_player(int id, const char *team_name, int x, int y)
{
    player_t *player = malloc(sizeof(player_t));

    if (player == NULL) {
        fprintf(stderr, "Memory allocation failed for player.\n");
        return NULL;
    }
    player->team_name = strdup(team_name);
    set_player_defaults(player, id, x, y);
    init_player_resources(player);
    init_player_commands(player);
    return player;
}

void destroy_player(player_t *player)
{
    if (player == NULL)
        return;
    free(player->team_name);
    free(player);
}

void move_player(player_t *player, int x, int y, map_t *map)
{
    tile_t *old_tile = NULL;
    tile_t *new_tile = NULL;

    if (player == NULL || map == NULL)
        return;
    old_tile = get_tile(map, player->x, player->y);
    if (old_tile)
        remove_player_from_tile(old_tile, player);
    normalize_coordinates_toroidal(&x, &y, map->width, map->height);
    player->x = x;
    player->y = y;
    new_tile = get_tile_toroidal(map, player->x, player->y);
    if (new_tile)
        add_player_to_tile(new_tile, player);
}

bool player_decrement_food(player_t *player)
{
    if (player == NULL)
        return false;
    if (player->food > 0) {
        player->food--;
        return true;
    } else {
        player->dead = true;
        return false;
    }
}

bool has_pending_commands(player_t *player)
{
    if (!player)
        return false;
    return player->command_count > 0;
}

player_t *hatch_egg_for_client(server_t *server, client_t *client)
{
    egg_t *egg = find_team_egg(server, client->team_name);
    player_t *player = NULL;
    tile_t *tile = NULL;

    if (!egg)
        return NULL;
    egg->is_hatched = true;
    player = create_player(client->id, client->team_name,
        egg->x, egg->y);
    if (player) {
        player->orientation = rand() % 4;
        egg->player_id = player->id;
        tile = get_tile_toroidal(server->map, egg->x, egg->y);
        if (tile)
            add_player_to_tile(tile, player);
        send_ebo(server, egg->id);
        send_pnw(server, player);
    }
    return player;
}
