/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Poll Operations
*/

#include "zappy_net_internal.h"
#include <poll.h>

static void build_poll_bitmaps(struct pollfd *poll_fds, int count,
                              zn_poll_result_t *result);
static void check_poll_errors(struct pollfd *poll_fds, int index,
                              uint64_t mask, zn_poll_result_t *result);

static int validate_poll_params(zn_socket_t *sockets, short *events,
                                int count, zn_poll_result_t *result);
static void setup_poll_fds(zn_socket_t *sockets, short *events, int count,
                           struct pollfd *poll_fds, zn_poll_result_t *result);
static int execute_poll(struct pollfd *poll_fds, int count, int timeout_ms);

int zn_get_fd(zn_socket_t socket)
{
    if (!socket)
        return -1;
    return socket->fd;
}

zn_poll_result_t zn_poll(zn_socket_t *sockets, short *events,
                        int count, int timeout_ms)
{
    zn_poll_result_t result = {0, 0, 0, 0};
    struct pollfd poll_fds[ZN_POLL_MAX_SOCKETS];
    int ready;

    if (validate_poll_params(sockets, events, count, &result) < 0)
        return result;
    setup_poll_fds(sockets, events, count, poll_fds, &result);
    ready = execute_poll(poll_fds, count, timeout_ms);
    if (ready <= 0)
        return result;
    build_poll_bitmaps(poll_fds, count, &result);
    return result;
}

static void build_poll_bitmaps(struct pollfd *poll_fds, int count,
                              zn_poll_result_t *result)
{
    uint64_t mask;

    for (int i = 0; i < count; i++) {
        mask = 1ULL << i;
        if (poll_fds[i].revents & (POLLIN | POLLPRI)) {
            result->readable |= mask;
            result->ready_count++;
        }
        if (poll_fds[i].revents & POLLOUT) {
            result->writable |= mask;
            result->ready_count++;
        }
        check_poll_errors(poll_fds, i, mask, result);
    }
}

static void check_poll_errors(struct pollfd *poll_fds, int index,
                              uint64_t mask, zn_poll_result_t *result)
{
    if (poll_fds[index].revents & (POLLERR | POLLHUP | POLLNVAL)) {
        result->error |= mask;
        result->ready_count++;
    }
}

static int validate_poll_params(zn_socket_t *sockets, short *events,
                                int count, zn_poll_result_t *result)
{
    if (!sockets || !events || count <= 0 || count > ZN_POLL_MAX_SOCKETS) {
        result->error = ~0ULL;
        return -1;
    }
    return 0;
}

static void setup_poll_fds(zn_socket_t *sockets, short *events, int count,
                           struct pollfd *poll_fds, zn_poll_result_t *result)
{
    for (int i = 0; i < count; i++) {
        if (!sockets[i] || sockets[i]->fd < 0) {
            result->error |= (1ULL << i);
            continue;
        }
        poll_fds[i].fd = sockets[i]->fd;
        poll_fds[i].events = events[i];
        poll_fds[i].revents = 0;
    }
}

static int execute_poll(struct pollfd *poll_fds, int count, int timeout_ms)
{
    int ready;

    do {
        ready = poll(poll_fds, count, timeout_ms);
    } while (ready == -1 && errno == EINTR);
    return ready;
}
