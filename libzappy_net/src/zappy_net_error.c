/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Zappy Network Library - Error handling implementation
*/

#include "zappy_net_error.h"
#include <stddef.h>

static const zn_error_entry_t error_messages[] = {
    {ZN_SUCCESS, "Success"},
    {ZN_ERROR_INVALID_PARAMS, "Invalid parameters"},
    {ZN_ERROR_MEMORY_ALLOCATION, "Memory allocation failed"},
    {ZN_ERROR_SOCKET_CREATION, "Socket creation failed"},
    {ZN_ERROR_SOCKET_BIND, "Socket bind failed"},
    {ZN_ERROR_SOCKET_LISTEN, "Socket listen failed"},
    {ZN_ERROR_SOCKET_CONNECT, "Socket connect failed"},
    {ZN_ERROR_SOCKET_ACCEPT, "Socket accept failed"},
    {ZN_ERROR_SOCKET_SEND, "Socket send failed"},
    {ZN_ERROR_SOCKET_RECEIVE, "Socket receive failed"},
    {ZN_ERROR_BUFFER_FULL, "Buffer is full"},
    {ZN_ERROR_BUFFER_EMPTY, "Buffer is empty"},
    {ZN_ERROR_CONNECTION_LOST, "Connection lost"},
    {ZN_ERROR_HOSTNAME_RESOLUTION, "Hostname resolution failed"},
    {ZN_ERROR_TIMEOUT, "Operation timed out"},
    {ZN_ERROR_UNKNOWN, "Unknown error"}
};

static const size_t error_messages_count = sizeof(error_messages) /
sizeof(error_messages[0]);

static const char *lookup_error_message(zn_err_t code)
{
    size_t i;

    for (i = 0; i < error_messages_count; i++) {
        if (error_messages[i].code == code) {
            return error_messages[i].message;
        }
    }
    return NULL;
}

const char *zn_strerror(zn_err_t code)
{
    const char *msg = lookup_error_message(code);

    return msg ? msg : "Unknown error";
}
