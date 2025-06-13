/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Test for unified error handling API
*/

#include <criterion/criterion.h>
#include "../../libzappy_net/include/zappy_net.h"

Test(error_handling, test_success_result)
{
    zn_result_t result = ZN_OK(42);

    cr_assert(ZN_IS_SUCCESS(result), "Result should be successful");
    cr_assert_eq(result.success, 42, "Success value should be 42");
}

Test(error_handling, test_error_result)
{
    zn_result_t result = ZN_ERR(ZN_ERROR_INVALID_PARAMS);

    cr_assert(ZN_IS_ERROR(result), "Result should be an error");
    cr_assert_eq(result.error, ZN_ERROR_INVALID_PARAMS,
        "Error code should be ZN_ERROR_INVALID_PARAMS");
}

Test(error_handling, test_error_messages)
{
    const char *msg = zn_strerror(ZN_SUCCESS);

    cr_assert_str_eq(msg, "Success", "Success message should be correct");

    msg = zn_strerror(ZN_ERROR_INVALID_PARAMS);
    cr_assert_str_eq(msg, "Invalid parameters",
        "Invalid params message should be correct");

    msg = zn_strerror(ZN_ERROR_SOCKET_CREATION);
    cr_assert_str_eq(msg, "Socket creation failed",
        "Socket creation message should be correct");
}

Test(error_handling, test_unknown_error)
{
    const char *msg = zn_strerror((zn_err_t)999);

    cr_assert_str_eq(msg, "Unknown error",
        "Unknown error should return generic message");
}

Test(error_handling, test_all_error_codes)
{
    const char *msg;

    msg = zn_strerror(ZN_ERROR_MEMORY_ALLOCATION);
    cr_assert_str_eq(msg, "Memory allocation failed");

    msg = zn_strerror(ZN_ERROR_SOCKET_BIND);
    cr_assert_str_eq(msg, "Socket bind failed");

    msg = zn_strerror(ZN_ERROR_SOCKET_LISTEN);
    cr_assert_str_eq(msg, "Socket listen failed");

    msg = zn_strerror(ZN_ERROR_SOCKET_CONNECT);
    cr_assert_str_eq(msg, "Socket connect failed");

    msg = zn_strerror(ZN_ERROR_SOCKET_ACCEPT);
    cr_assert_str_eq(msg, "Socket accept failed");

    msg = zn_strerror(ZN_ERROR_SOCKET_SEND);
    cr_assert_str_eq(msg, "Socket send failed");

    msg = zn_strerror(ZN_ERROR_SOCKET_RECEIVE);
    cr_assert_str_eq(msg, "Socket receive failed");

    msg = zn_strerror(ZN_ERROR_BUFFER_FULL);
    cr_assert_str_eq(msg, "Buffer is full");

    msg = zn_strerror(ZN_ERROR_BUFFER_EMPTY);
    cr_assert_str_eq(msg, "Buffer is empty");

    msg = zn_strerror(ZN_ERROR_CONNECTION_LOST);
    cr_assert_str_eq(msg, "Connection lost");

    msg = zn_strerror(ZN_ERROR_HOSTNAME_RESOLUTION);
    cr_assert_str_eq(msg, "Hostname resolution failed");

    msg = zn_strerror(ZN_ERROR_TIMEOUT);
    cr_assert_str_eq(msg, "Operation timed out");

    msg = zn_strerror(ZN_ERROR_UNKNOWN);
    cr_assert_str_eq(msg, "Unknown error");
}
