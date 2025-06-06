/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_fill_p_x_y
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../server/includes/server.h"

extern char *optarg1;
char *optarg1;

Test(fill_port, valid_port)
{
    server_args_t server = {0};
    char *mock_optarg1 = "8080";

    optarg1 = mock_optarg1;
    fill_port(&server, optarg1);
    cr_assert_eq(server.port, 8080, "Port should be set to 8080");
    cr_assert_eq(server.error_code, 0, "Error code should be 0 for valid port");
}

Test(fill_port, invalid_port)
{
    server_args_t server = {0};
    char *mock_optarg1 = "70000";

    optarg1 = mock_optarg1;
    fill_port(&server, optarg1);
    cr_assert_eq(server.error_code, 84, "Error code should be 84 for invalid port");
}

Test(fill_width, valid_width)
{
    server_args_t server = {0};
    char *mock_optarg1 = "100";

    optarg1 = mock_optarg1;
    fill_witdh(&server, optarg1);
    cr_assert_eq(server.width, 100, "Width should be set to 100");
    cr_assert_eq(server.error_code, 0, "Error code should be 0 for valid width");
}

Test(fill_width, invalid_width)
{
    server_args_t server = {0};
    char *mock_optarg1 = "0";

    optarg1 = mock_optarg1;
    fill_witdh(&server, optarg1);
    cr_assert_eq(server.error_code, 84, "Error code should be 84 for invalid width");
}

Test(fill_height, valid_height)
{
    server_args_t server = {0};
    char *mock_optarg1 = "200";

    optarg1 = mock_optarg1;
    fill_height(&server, optarg1);
    cr_assert_eq(server.height, 200, "Height should be set to 200");
    cr_assert_eq(server.error_code, 0, "Error code should be 0 for valid height");
}

Test(fill_height, invalid_height)
{
    server_args_t server = {0};
    char *mock_optarg1 = "0";

    optarg1 = mock_optarg1;
    fill_height(&server, optarg1);
    cr_assert_eq(server.error_code, 84, "Error code should be 84 for invalid height");
}
