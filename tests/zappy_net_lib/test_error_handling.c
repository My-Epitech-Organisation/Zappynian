/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Test for unified error handling API
*/

#include <criterion/criterion.h>
#include <pthread.h>
#include "zappy_net_error.h"

Test(error_handling, test_initial_state)
{
    zn_err_t error = zn_last_error();

    cr_assert_eq(error, ZN_SUCCESS, "Initial error state should be SUCCESS");
}

Test(error_handling, test_set_and_get_error)
{
    zn_set_error(ZN_ERROR_INVALID_PARAMS);

    zn_err_t error = zn_last_error();

    cr_assert_eq(error, ZN_ERROR_INVALID_PARAMS,
        "Error should be set correctly");
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

static void *thread_test_func(void *arg)
{
    int *result = (int *)arg;

    zn_set_error(ZN_ERROR_MEMORY_ALLOCATION);
    *result = (zn_last_error() == ZN_ERROR_MEMORY_ALLOCATION) ? 1 : 0;
    return NULL;
}

Test(error_handling, test_thread_local_errors)
{
    pthread_t thread;
    int thread_result = 0;

    zn_set_error(ZN_ERROR_SOCKET_CREATION);
    pthread_create(&thread, NULL, thread_test_func, &thread_result);
    pthread_join(thread, NULL);

    cr_assert_eq(thread_result, 1,
        "Thread should have its own error state");
    cr_assert_eq(zn_last_error(), ZN_ERROR_SOCKET_CREATION,
        "Main thread error should be preserved");
}
