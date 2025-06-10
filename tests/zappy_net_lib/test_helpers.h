/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test helper functions and internal structure access
*/

#ifndef TEST_HELPERS_H_
    #define TEST_HELPERS_H_

#include "../../libzappy_net/include/zappy_net.h"
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @brief Test helper to get socket file descriptor
 * @param sock Socket handle
 * @return File descriptor or -1 on error
 */
int test_get_socket_fd(zn_socket_t sock);

/**
 * @brief Test helper to check if socket is initialized
 * @param sock Socket handle
 * @return 1 if initialized, 0 otherwise
 */
int test_is_socket_initialized(zn_socket_t sock);

#endif /* !TEST_HELPERS_H_ */
