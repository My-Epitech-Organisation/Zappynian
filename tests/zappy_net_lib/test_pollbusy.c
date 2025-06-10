/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Performance test for zn_poll - CPU usage benchmark
*/

#include <criterion/criterion.h>
#include "../../libzappy_net/include/zappy_net.h"
#include <time.h>
#include <sys/resource.h>
#include <poll.h>

static double get_cpu_usage(struct rusage *start, struct rusage *end,
                           double elapsed_time)
{
    double user_time = (end->ru_utime.tv_sec - start->ru_utime.tv_sec) +
                      (end->ru_utime.tv_usec - start->ru_utime.tv_usec) / 1000000.0;
    double sys_time = (end->ru_stime.tv_sec - start->ru_stime.tv_sec) +
                     (end->ru_stime.tv_usec - start->ru_stime.tv_usec) / 1000000.0;

    return ((user_time + sys_time) / elapsed_time) * 100.0;
}

Test(zn_poll_performance, cpu_usage_idle, .timeout = 10)
{
    zn_socket_t server = zn_server_listen(0);
    cr_assert_not_null(server, "Failed to create server socket");

    zn_socket_t sockets[5] = {server, server, server, server, server};
    short events[5] = {POLLIN, POLLIN, POLLIN, POLLIN, POLLIN};

    struct rusage usage_start, usage_end;
    getrusage(RUSAGE_SELF, &usage_start);

    time_t start_time = time(NULL);
    int iterations = 0;

    while (time(NULL) - start_time < 5) {
        zn_poll_result_t result = zn_poll(sockets, events, 5, 100);
        iterations++;

        cr_assert_eq(result.ready_count, 0,
                    "Expected no ready sockets in idle test");
        cr_assert_eq(result.readable, 0,
                    "Expected no readable sockets in idle test");
        cr_assert_eq(result.writable, 0,
                    "Expected no writable sockets in idle test");
        cr_assert_eq(result.error, 0,
                    "Expected no error sockets in idle test");
    }

    getrusage(RUSAGE_SELF, &usage_end);
    double elapsed_time = 5.0;
    double cpu_usage = get_cpu_usage(&usage_start, &usage_end, elapsed_time);

    cr_log_info("CPU usage over 5s: %.2f%% (%d iterations)",
           cpu_usage, iterations);
    cr_log_info("Average time per poll: %.2f ms",
           (elapsed_time * 1000.0) / iterations);

    cr_assert_lt(cpu_usage, 3.0,
               "CPU usage too high: %.2f%% (expected < 3%%)", cpu_usage);

    zn_close(server);
}

Test(zn_poll_functionality, bitmap_return)
{
    zn_socket_t server = zn_server_listen(0);
    cr_assert_not_null(server, "Failed to create server socket");

    zn_socket_t sockets[1] = {server};
    short events[1] = {POLLIN};

    zn_poll_result_t result = zn_poll(sockets, events, 1, 0);

    cr_assert_eq(result.readable, 0, "Server should not be ready for reading");
    cr_assert_eq(result.ready_count, 0, "No sockets should be ready");

    zn_close(server);
}

Test(zn_poll_edge_cases, null_parameters)
{
    zn_poll_result_t result;
    zn_socket_t socket = zn_server_listen(0);
    short events = POLLIN;

    result = zn_poll(NULL, &events, 1, 0);
    cr_assert_eq(result.error, ~0ULL, "Expected error for NULL sockets");

    result = zn_poll(&socket, NULL, 1, 0);
    cr_assert_eq(result.error, ~0ULL, "Expected error for NULL events");

    result = zn_poll(&socket, &events, 0, 0);
    cr_assert_eq(result.error, ~0ULL, "Expected error for zero count");

    result = zn_poll(&socket, &events, ZN_POLL_MAX_SOCKETS + 1, 0);
    cr_assert_eq(result.error, ~0ULL, "Expected error for too many sockets");

    zn_close(socket);
}

Test(zn_poll_timeout, behavior, .timeout = 5)
{
    zn_socket_t server = zn_server_listen(0);
    cr_assert_not_null(server, "Failed to create server");

    short events = POLLIN;
    time_t start_time = time(NULL);

    zn_poll_result_t result = zn_poll(&server, &events, 1, 1000);

    time_t elapsed = time(NULL) - start_time;

    cr_assert_geq(elapsed, 1, "Timeout should take at least 1 second");
    cr_assert_leq(elapsed, 2, "Timeout should not take more than 2 seconds");
    cr_assert_eq(result.ready_count, 0, "No sockets should be ready");

    zn_close(server);
}
