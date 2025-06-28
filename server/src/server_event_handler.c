/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server client event handling functions
*/

#include "../include/server.h"
#include "../include/team.h"
#include "../include/egg.h"

static int assign_player_to_team(team_t *team, player_t *player)
{
    int i = 0;

    for (i = 0; i < team->max_slots; i++) {
        if (team->players[i] == NULL) {
            team->players[i] = player;
            team->current_players++;
            return 1;
        }
    }
    return 0;
}

static void send_connection_responses(client_t *client, team_t *team,
    server_t *server)
{
    char response[256];

    snprintf(response, sizeof(response), "%d", team->remaining_slots);
    zn_send_message(client->zn_sock, response);
    snprintf(response, sizeof(response), "%d %d",
        (int)server->args->width, (int)server->args->height);
    zn_send_message(client->zn_sock, response);
}

static void handle_ia_connection(server_t *server, int client_idx)
{
    client_t *client = server->connection->clients[client_idx];
    team_t *team = NULL;
    player_t *player = NULL;

    if (client == NULL || client->team_name == NULL)
        return;
    team = get_team_by_name(server->args, client->team_name);
    if (team == NULL)
        return;
    player = hatch_egg_for_client(server, client);
    if (player == NULL)
        player = create_player_for_client(server, client, team, client->id);
    else {
        player->slot_id = client->id;
        if (!assign_player_to_team(team, player))
            return;
    }
    if (player != NULL)
        send_connection_responses(client, team, server);
}

static void handle_gui_connection(server_t *server, int client_idx)
{
    client_t *client = server->connection->clients[client_idx];

    if (client == NULL || server->graphic_clients == NULL) {
        return;
    }
    if (add_graphic_client(server->graphic_clients, client) == 0) {
        printf("Graphic client %d added to the list\n", client->id);
    } else {
        printf("Failed to add graphic client %d to the list\n", client->id);
    }
}

static void handle_disconnection(server_t *server, int client_idx)
{
    client_t *client = server->connection->clients[client_idx];

    if (client == NULL) {
        return;
    }
    if (client->type == CLIENT_GUI && server->graphic_clients != NULL) {
        if (remove_graphic_client(server->graphic_clients, client) == 0) {
            printf("Graphic client %d removed from the list\n", client->id);
        } else {
            printf("Failed to remove graphic client %d from the list\n",
                client->id);
        }
    }
}

void handle_client_event(server_t *server, client_event_t event,
    int client_idx)
{
    switch (event) {
        case CLIENT_EVENT_IA_CONNECTED:
            handle_ia_connection(server, client_idx);
            break;
        case CLIENT_EVENT_GUI_CONNECTED:
            handle_gui_connection(server, client_idx);
            break;
        case CLIENT_EVENT_DISCONNECTED:
            handle_disconnection(server, client_idx);
            break;
        default:
            break;
    }
}
