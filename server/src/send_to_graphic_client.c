/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** send_to_graphic_client
*/

#include "../include/server.h"

void send_to_all_graphic_clients(graphic_client_list_t *list,
    const char *message)
{
    graphic_client_node_t *current;

    if (list == NULL || message == NULL) {
        return;
    }
    current = list->head;
    while (current != NULL) {
        if (current->client != NULL && current->client->zn_sock != NULL) {
            zn_send_message(current->client->zn_sock, message);
        }
        current = current->next;
    }
}
