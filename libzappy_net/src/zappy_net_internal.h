/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Internal Header
*/

#ifndef ZAPPY_NET_INTERNAL_H_
    #define ZAPPY_NET_INTERNAL_H_

    #include "../include/zappy_net.h"
    #include <netinet/in.h>

/**
 * @brief Internal socket structure definition
 */
struct zn_socket {
    int fd;
    int initialized;
    struct sockaddr_in addr;
    int type;
};

#endif /* !ZAPPY_NET_INTERNAL_H_ */
