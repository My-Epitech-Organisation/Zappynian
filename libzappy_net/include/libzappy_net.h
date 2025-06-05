/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network interface header
*/

#ifndef LIBZAPPY_NET_H
#define LIBZAPPY_NET_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the network library
 *
 * @return int 0 on success, -1 on failure
 */
int zappy_net_init(void);

/**
 * @brief Cleanup the network library
 */
void zappy_net_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* LIBZAPPY_NET_H */
