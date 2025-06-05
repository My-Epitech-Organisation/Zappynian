/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_display_infos
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../includes/server.h"

Test(display_infos, check_display_infos)
{
    server_config_t server;
    server.port = 8080;
    server.width = 10;
    server.height = 10;
    server.frequency = 1;
    server.clients_per_team = 5;
    char *teams[] = {"team1", "team2", NULL};
    server.team_count = 2;
    server.team_names = teams;

    cr_redirect_stdout();
    display_infos(&server);
    fflush(stdout);
    cr_assert_stdout_eq_str("Server configuration:\nPort: 8080\nWidth: 10\nHeight: 10\nTeams:\n - team1\n - team2\nClients per team: 5\nFrequency: 1\n",
        "Output should match expected display format");
}
