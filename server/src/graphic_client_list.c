/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Graphic clients linked list management
*/

#include "../include/server.h"

graphic_client_list_t *create_graphic_client_list(void)
{
    graphic_client_list_t *list = malloc(sizeof(graphic_client_list_t));

    if (list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

void destroy_graphic_client_list(graphic_client_list_t *list)
{
    graphic_client_node_t *current;
    graphic_client_node_t *next;

    if (list == NULL) {
        return;
    }
    current = list->head;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

int add_graphic_client(graphic_client_list_t *list, client_t *client)
{
    graphic_client_node_t *new_node;

    if (list == NULL || client == NULL) {
        return -1;
    }
    if (client->type != CLIENT_GUI) {
        return -1;
    }
    new_node = malloc(sizeof(graphic_client_node_t));
    if (new_node == NULL) {
        return -1;
    }
    new_node->client = client;
    new_node->next = list->head;
    list->head = new_node;
    list->count++;
    return 0;
}

static void remove_node(graphic_client_list_t *list,
    graphic_client_node_t *current, graphic_client_node_t *prev)
{
    if (prev == NULL)
        list->head = current->next;
    else
        prev->next = current->next;
    free(current);
    list->count--;
}

static graphic_client_node_t *find_client_node(graphic_client_list_t *list,
    client_t *client, graphic_client_node_t **prev)
{
    graphic_client_node_t *current = list->head;

    *prev = NULL;
    while (current != NULL) {
        if (current->client == client)
            return current;
        *prev = current;
        current = current->next;
    }
    return NULL;
}

int remove_graphic_client(graphic_client_list_t *list, client_t *client)
{
    graphic_client_node_t *prev;
    graphic_client_node_t *current;

    if (list == NULL || client == NULL)
        return -1;
    current = find_client_node(list, client, &prev);
    if (current == NULL)
        return -1;
    remove_node(list, current, prev);
    return 0;
}

graphic_client_node_t *find_graphic_client(graphic_client_list_t *list,
    int client_id)
{
    graphic_client_node_t *current;

    if (list == NULL) {
        return NULL;
    }
    current = list->head;
    while (current != NULL) {
        if (current->client->id == client_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
