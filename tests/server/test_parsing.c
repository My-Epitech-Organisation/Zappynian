/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_parsing
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../server/include/server.h"

Test(handle_options, valid_option_p)
{
    server_args_t server = {0};
    char *mock_argv[] = {"./zappy_server", "-p", "8080", NULL};
    int mock_argc = 3;

    optarg = "8080";
    int result = handle_options('p', mock_argv, mock_argc, &server);
    cr_assert_eq(result, 0, "handle_options should return 0 for valid port");
    cr_assert_eq(server.port, 8080, "Port should be set to 8080");
}

Test(handle_options, invalid_option_p)
{
    server_args_t server = {0};
    char *mock_argv[] = {"./zappy_server", "-p", "invalid_port", NULL};
    int mock_argc = 3;

    optarg = "invalid_port";
    int result = handle_options('p', mock_argv, mock_argc, &server);
    cr_assert_eq(result, 84, "handle_options should return 84 for invalid port");
    cr_assert_eq(server.error_code, 84, "Error code should be set to 84 for invalid port");
}

Test(handle_options, valid_option_c)
{
    server_args_t server = {0};
    char *argv[] = {"./zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "Team1", "-c", "invalid_clients", "-f", "5", NULL};
    int argc = 13;
    char *opt_string = "p:x:y:n:c:f:";
    int opt = 0;

    opt = getopt(argc, argv, opt_string);
    int result = handle_options(opt, argv, argc, &server);
    cr_assert_eq(result, 0, "handle_options should return 0 for valid clients number");
}

Test(handle_options, invalid_option_c)
{
    server_args_t server = {0};
    char *argv[] = {"./zappy_server", "-p", "70000", "-x", "10", "-y", "10", "-n", "Team1", "-c", "invalid_clients", "-f", "5", NULL};
    int argc = 13;
    char *opt_string = "p:x:y:n:c:f:";
    int opt = 0;

    opt = getopt(argc, argv, opt_string);
    int result = handle_options(opt, argv, argc, &server);
    cr_assert_eq(result, 84, "handle_options should return 84 for invalid clients number");
    cr_assert_eq(server.error_code, 84, "Error code should be set to 84 for invalid clients number");
}
