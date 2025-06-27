/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Server client event handling functions
*/

#include "../include/server.h"
#include "../include/team.h"

static void handle_ia_connection(server_t *server, int client_idx)
{
    client_t *client = server->connection->clients[client_idx];
    team_t *team = NULL;
    player_t *player = NULL;
    char response[256];

    if (client == NULL || client->team_name == NULL)
        return;
    team = get_team_by_name(server->args, client->team_name);
    if (team == NULL)
        return;
    player = create_player_for_client(server, client, team);
    if (player == NULL)
        return;
    send_pnw(server, player);
    snprintf(response, sizeof(response), "%d", team->remaining_slots);
    zn_send_message(client->zn_sock, response);
    snprintf(response, sizeof(response), "%d %d",
        (int)server->args->width, (int)server->args->height);
    zn_send_message(client->zn_sock, response);
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
