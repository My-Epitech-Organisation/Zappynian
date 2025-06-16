/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_main
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../server/include/server.h"

Test(main, check_memory_allocation)
{
    server_t *server = malloc(sizeof(server_t));

    cr_assert_not_null(server, "Memory allocation for server_args_t failed");
    free(server);
}

Test(main, check_handle_args_return_value)
{
    char *argv[] = {"./zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "1"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    server_t server;
    int result = handle_args(argc, argv, &server);

    cr_assert_eq(result, 0, "handle_args should return 0 for valid arguments");
}

Test(main, check_handle_args_invalid_port)
{
    char *argv[] = {"./zappy_server", "-p", "invalid", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "5", "-f", "1"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    server_t server;
    int result = handle_args(argc, argv, &server);

    cr_assert_eq(result, 84, "handle_args should return 84 for invalid port");
}

Test(main, check_handle_args_missing_arguments)
{
    char *argv[] = {"./zappy_server", "-p", "8080"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    server_t server;
    int result = handle_args(argc, argv, &server);

    cr_assert_eq(result, 84, "handle_args should return 84 for missing arguments");
}

Test(main, check_handle_args_help_flag)
{
    char *argv[] = {"./zappy_server", "-help"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    server_t server;
    int result = handle_args(argc, argv, &server);

    cr_assert_eq(result, 1, "handle_args should return 1 for help flag");
}
