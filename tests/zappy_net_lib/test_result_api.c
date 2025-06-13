/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Test for the new zn_result_t API
*/

#include <criterion/criterion.h>
#include "../../libzappy_net/include/zappy_net.h"

TestSuite(result_api);

Test(result_api, test_success_creation)
{
    zn_result_t result = ZN_OK(42);

    cr_assert(ZN_IS_SUCCESS(result), "Result should be successful");
    cr_assert_eq(result.success, 42, "Success value should be 42");
    cr_assert(!ZN_IS_ERROR(result), "Result should not be an error");
}

Test(result_api, test_error_creation)
{
    zn_result_t result = ZN_ERR(ZN_ERROR_INVALID_PARAMS);

    cr_assert(ZN_IS_ERROR(result), "Result should be an error");
    cr_assert_eq(result.error, ZN_ERROR_INVALID_PARAMS,
        "Error code should be ZN_ERROR_INVALID_PARAMS");
    cr_assert(!ZN_IS_SUCCESS(result), "Result should not be successful");
}

Test(result_api, test_success_with_pointer)
{
    char *test_data = "Hello, World!";
    zn_result_t result = ZN_OK((intptr_t)test_data);

    cr_assert(ZN_IS_SUCCESS(result), "Result should be successful");
    cr_assert_eq((char *)result.success, test_data,
        "Success value should be the test string");
}

Test(result_api, test_error_with_different_codes)
{
    zn_result_t result1 = ZN_ERR(ZN_ERROR_SOCKET_CREATION);
    zn_result_t result2 = ZN_ERR(ZN_ERROR_MEMORY_ALLOCATION);

    cr_assert(ZN_IS_ERROR(result1), "First result should be an error");
    cr_assert(ZN_IS_ERROR(result2), "Second result should be an error");
    cr_assert_eq(result1.error, ZN_ERROR_SOCKET_CREATION,
        "First error code should be SOCKET_CREATION");
    cr_assert_eq(result2.error, ZN_ERROR_MEMORY_ALLOCATION,
        "Second error code should be MEMORY_ALLOCATION");
}

Test(result_api, test_error_message_integration)
{
    zn_result_t result = ZN_ERR(ZN_ERROR_CONNECTION_LOST);
    const char *msg = zn_strerror(result.error);

    cr_assert(ZN_IS_ERROR(result), "Result should be an error");
    cr_assert_str_eq(msg, "Connection lost",
        "Error message should be correct");
}

Test(result_api, test_success_with_zero)
{
    zn_result_t result = ZN_OK(0);

    cr_assert(ZN_IS_SUCCESS(result), "Result with zero should be successful");
    cr_assert_eq(result.success, 0, "Success value should be 0");
}

Test(result_api, test_macro_behavior)
{
    // Test that the macros work correctly
    zn_result_t success_result = ZN_OK(123);
    zn_result_t error_result = ZN_ERR(ZN_ERROR_TIMEOUT);

    // Success tests
    cr_assert(ZN_IS_SUCCESS(success_result));
    cr_assert(!ZN_IS_ERROR(success_result));

    // Error tests
    cr_assert(ZN_IS_ERROR(error_result));
    cr_assert(!ZN_IS_SUCCESS(error_result));
}
