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
    * @brief Result structure containing both success/failure and error code
    *
    * This structure is returned by all library functions to provide
    * both the operation result and detailed error information.
    */
    typedef struct zn_result_s {
        int success;
        zn_err_t error;
    } zn_result_t;

    /**
    * @brief Create a successful result
    *
    * @param result The success value (typically 0 or positive)
    * @return A zn_result_t indicating success
    */
    #define ZN_OK(result) ((zn_result_t){.success = (result), .error = ZN_SUCCESS})

    /**
    * @brief Create an error result
    *
    * @param code The error code
    * @return A zn_result_t indicating failure with the given error code
    */
    #define ZN_ERR(code) ((zn_result_t){.success = -1, .error = (code)})

    /**
    * @brief Check if a result indicates success
    *
    * @param result The result to check
    * @return Non-zero if successful, 0 if failed
    */
    #define ZN_IS_SUCCESS(result) ((result).success >= 0)

    /**
    * @brief Check if a result indicates failure
    *
    * @param result The result to check
    * @return Non-zero if failed, 0 if successful
    */
    #define ZN_IS_ERROR(result) ((result).success < 0)

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

    #ifdef __cplusplus
}
    #endif

#endif /* !ZAPPY_NET_ERROR_H */
