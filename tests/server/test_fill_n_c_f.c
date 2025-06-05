/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_fill_n_c_f
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../server/includes/server.h"

extern char *optarg;
char *optarg;

Test(fill_name, single_team_name)
{
    server_config_t server = {0};
    char *mock_argv[] = {"./zappy_server", "-n", "Team1", NULL};
    int mock_argc = 3;

    optarg = "Team1";
    optind = 3;
    fill_name(&server, mock_argc, mock_argv);
    cr_assert_not_null(server.team_names, "Team names array should be allocated");
    cr_assert_str_eq(server.team_names[0], "Team1", "First team name should be 'Team1'");
    cr_assert_eq(server.team_count, 1, "Team count should be 1");
    free(server.team_names[0]);
    free(server.team_names);
}

Test(fill_frequency, valid_frequency)
{
    server_config_t server = {0};
    char *mock_optarg = "10";

    optarg = mock_optarg;
    fill_frequency(&server, optarg);
    cr_assert_eq(server.frequency, 10, "Frequency should be set to 10");
    cr_assert_eq(server.error_code, 0, "Error code should be 0 for valid frequency");
}

Test(fill_frequency, invalid_frequency)
{
    server_config_t server = {0};
    char *mock_optarg = "0";

    optarg = mock_optarg;
    fill_frequency(&server, optarg);
    cr_assert_eq(server.frequency, 0, "Frequency should be set to 0 for invalid input");
    cr_assert_eq(server.error_code, 84, "Error code should be 84 for invalid frequency");
}

Test(fill_clients_nb, valid_clients_number)
{
    server_config_t server = {0};
    char *mock_optarg = "5";

    optarg = mock_optarg;
    fill_clients_nb(&server, optarg);
    cr_assert_eq(server.clients_per_team, 5, "Clients per team should be set to 5");
    cr_assert_eq(server.error_code, 0, "Error code should be 0 for valid clients number");
}

Test(fill_clients_nb, invalid_clients_number)
{
    server_config_t server = {0};
    char *mock_optarg = "0";

    optarg = mock_optarg;
    fill_clients_nb(&server, optarg);
    cr_assert_eq(server.clients_per_team, 0, "Clients per team should be set to 0 for invalid input");
    cr_assert_eq(server.error_code, 84, "Error code should be 84 for invalid clients number");
}
