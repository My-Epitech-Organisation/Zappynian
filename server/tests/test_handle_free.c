/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_handle_free
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../includes/server.h"

Test(handle_free, free_server_with_team_names)
{
    server_config_t *server = malloc(sizeof(server_config_t));
    server->team_names = malloc(2 * sizeof(char *));
    server->team_names[0] = malloc(10);
    server->team_names[1] = NULL;

    strcpy(server->team_names[0], "Team1");

    cr_assert_eq(handle_free(server), 0, "handle_free should return 0");
}

Test(handle_free, free_server_without_team_names)
{
    server_config_t *server = malloc(sizeof(server_config_t));
    server->team_names = NULL;

    cr_assert_eq(handle_free(server), 0, "handle_free should return 0");
}
