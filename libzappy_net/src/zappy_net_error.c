/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Zappy Network Library - Error handling implementation
*/

#include "zappy_net_error.h"
#include <stddef.h>

static const char *get_basic_error_msg(zn_err_t code)
{
    if (code == ZN_SUCCESS)
        return "Success";
    if (code == ZN_ERROR_INVALID_PARAMS)
        return "Invalid parameters";
    if (code == ZN_ERROR_MEMORY_ALLOCATION)
        return "Memory allocation failed";
    return NULL;
}

static const char *get_socket_creation_msg(zn_err_t code)
{
    if (code == ZN_ERROR_SOCKET_CREATION)
        return "Socket creation failed";
    if (code == ZN_ERROR_SOCKET_BIND)
        return "Socket bind failed";
    if (code == ZN_ERROR_SOCKET_LISTEN)
        return "Socket listen failed";
    if (code == ZN_ERROR_SOCKET_CONNECT)
        return "Socket connect failed";
    return NULL;
}

static const char *get_socket_io_msg(zn_err_t code)
{
    if (code == ZN_ERROR_SOCKET_ACCEPT)
        return "Socket accept failed";
    if (code == ZN_ERROR_SOCKET_SEND)
        return "Socket send failed";
    if (code == ZN_ERROR_SOCKET_RECEIVE)
        return "Socket receive failed";
    return NULL;
}

static const char *get_buffer_error_msg(zn_err_t code)
{
    if (code == ZN_ERROR_BUFFER_FULL)
        return "Buffer is full";
    if (code == ZN_ERROR_BUFFER_EMPTY)
        return "Buffer is empty";
    return NULL;
}

static const char *get_connection_error_msg(zn_err_t code)
{
    if (code == ZN_ERROR_CONNECTION_LOST)
        return "Connection lost";
    if (code == ZN_ERROR_HOSTNAME_RESOLUTION)
        return "Hostname resolution failed";
    if (code == ZN_ERROR_TIMEOUT)
        return "Operation timed out";
    if (code == ZN_ERROR_UNKNOWN)
        return "Unknown error";
    return NULL;
}

static const char *resolve_error_message(zn_err_t code)
{
    const char *msg = get_basic_error_msg(code);

    if (msg == NULL)
        msg = get_socket_creation_msg(code);
    if (msg == NULL)
        msg = get_socket_io_msg(code);
    if (msg == NULL)
        msg = get_buffer_error_msg(code);
    if (msg == NULL)
        msg = get_connection_error_msg(code);
    return msg;
}

const char *zn_strerror(zn_err_t code)
{
    const char *msg = resolve_error_message(code);

    return msg ? msg : "Unknown error";
}
