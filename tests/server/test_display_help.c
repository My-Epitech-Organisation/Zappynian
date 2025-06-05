/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** test_display_help
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../server/includes/server.h"

Test(display_help, check_correct_return_value)
{
    cr_assert_eq(display_help(), 1, "Display help function should return 1");
}

Test(display_help, check_incorrect_return_value)
{
    cr_assert_neq(display_help(), 0,
        "Display help function should not return 0");
}

Test(display_help, output_contains_usage)
{
    cr_redirect_stdout();
    display_help();
    fflush(stdout);
    cr_assert_stdout_eq_str("Usage: ./zappy_server -p port -x width -y height -n name1 name2 ... -c clientsNb -f freq\n",
        "Output should contain usage information");
}
