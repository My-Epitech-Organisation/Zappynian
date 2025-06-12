/*
** EPITECH PROJECT, 2025
** Zappynian
** File description:
** Zappy Network Library - Error handling API
*/

#ifndef ZAPPY_NET_ERROR_H
    #define ZAPPY_NET_ERROR_H

    #ifdef __cplusplus
    extern "C" {
    #endif

    /**
    * @brief Error codes for the Zappy network library
    *
    * This enumeration defines all possible error codes that can be
    * returned by the Zappy network library functions.
    */
    typedef enum zn_err_e {
        ZN_SUCCESS = 0,
        ZN_ERROR_INVALID_PARAMS,
        ZN_ERROR_MEMORY_ALLOCATION,
        ZN_ERROR_SOCKET_CREATION,
        ZN_ERROR_SOCKET_BIND,
        ZN_ERROR_SOCKET_LISTEN,
        ZN_ERROR_SOCKET_CONNECT,
        ZN_ERROR_SOCKET_ACCEPT,
        ZN_ERROR_SOCKET_SEND,
        ZN_ERROR_SOCKET_RECEIVE,
        ZN_ERROR_BUFFER_FULL,
        ZN_ERROR_BUFFER_EMPTY,
        ZN_ERROR_CONNECTION_LOST,
        ZN_ERROR_HOSTNAME_RESOLUTION,
        ZN_ERROR_TIMEOUT,
        ZN_ERROR_UNKNOWN
    } zn_err_t;

    /**
    * @brief Get the last error that occurred in the current thread
    *
    * This function returns the error code of the last operation that
    * failed in the current thread. The error is thread-local, meaning
    * each thread has its own error state.
    *
    * @return The last error code, or ZN_SUCCESS if no error occurred
    */
    zn_err_t zn_last_error(void);

    /**
    * @brief Convert an error code to a human-readable string
    *
    * This function returns a string description of the given error code.
    * The returned string is statically allocated and should not be freed.
    *
    * @param code The error code to convert
    * @return String describing the error, or "Unknown error" for invalid codes
    */
    const char *zn_strerror(zn_err_t code);

    /**
    * @brief Set the last error for the current thread (internal use)
    *
    * This function is used internally by the library to set the error
    * state when an operation fails. It should not be called directly
    * by library users.
    *
    * @param error The error code to set
    */
    void zn_set_error(zn_err_t error);

    #ifdef __cplusplus
}
    #endif

#endif /* !ZAPPY_NET_ERROR_H */
