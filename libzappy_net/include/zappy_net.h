/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Network Library - Main Header
*/

#ifndef ZAPPY_NET_H
    #define ZAPPY_NET_H

    #include <stddef.h>
    #include <stdint.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <netdb.h>

    #ifdef __cplusplus
    extern "C" {
    #endif

    /**
    * @brief Opaque handle for network socket operations
    *
    * This structure represents an abstract socket handle used throughout
    * the Zappy network library. The internal implementation is hidden
    * to provide encapsulation and allow for future changes without
    * breaking the API.
    */
    typedef struct zn_socket *zn_socket_t;

    /**
    * @brief Initialize the Zappy network library
    *
    * This function must be called before using any other functions from
    * the Zappy network library. It initializes internal data structures
    * and prepares the library for network operations.
    *
    * @return 0 on success, -1 on failure
    */
    int zn_init(void);

    /**
    * @brief Clean up the Zappy network library
    *
    * This function should be called when the application is done using
    * the Zappy network library. It releases all resources allocated by
    * the library and performs necessary cleanup operations.
    */
    void zn_cleanup(void);

    /**
    * @brief Create a new socket handle
    *
    * Creates and initializes a new socket handle for network operations.
    * The returned handle must be destroyed with zn_socket_destroy() when
    * no longer needed.
    *
    * @return A valid socket handle on success, NULL on failure
    */
    zn_socket_t zn_socket_create(void);

    /**
    * @brief Destroy a socket handle
    *
    * Destroys a socket handle and releases all associated resources.
    * After calling this function, the socket handle becomes invalid
    * and must not be used.
    *
    * @param socket The socket handle to destroy
    */
    void zn_socket_destroy(zn_socket_t socket);

    /**
    * @brief Initialize a socket handle
    *
    * Initializes an existing socket handle for use. This function
    * prepares the socket for network operations but does not establish
    * any connections.
    *
    * @param socket The socket handle to initialize
    * @return 0 on success, -1 on failure
    */
    int zn_socket_init(zn_socket_t socket);

    /**
    * @brief Clean up a socket handle
    *
    * Cleans up a socket handle by closing any open connections and
    * releasing associated resources. The handle remains valid after
    * this operation but must be re-initialized before use.
    *
    * @param socket The socket handle to clean up
    */
    void zn_socket_cleanup(zn_socket_t socket);

    /**
    * @brief Create a server socket and listen on specified port
    *
    * Creates a TCP server socket, binds it to the specified port on all
    * available interfaces, and starts listening for incoming connections.
    * The socket is created with O_NONBLOCK flag.
    *
    * @param port The port number to listen on (1-65535)
    * @return A valid socket handle on success, NULL on failure
    */
    zn_socket_t zn_server_listen(int port);

    /**
    * @brief Connect to a server as a client
    *
    * Creates a TCP client socket and attempts to connect to the specified
    * host and port. The socket is created with O_NONBLOCK flag.
    * The connection may not be immediately established due to non-blocking nature.
    *
    * @param host The hostname or IP address to connect to
    * @param port The port number to connect to (1-65535)
    * @return A valid socket handle on success, NULL on failure
    */
    zn_socket_t zn_client_connect(const char *host, int port);

    /**
    * @brief Close a socket connection
    *
    * Closes the socket connection and cleans up the associated resources.
    * This function handles EINTR and EAGAIN appropriately.
    * After calling this function, the socket handle becomes invalid.
    *
    * @param socket The socket handle to close
    * @return 0 on success, -1 on failure
    */
    int zn_close(zn_socket_t socket);

#ifdef __cplusplus
}
#endif

#endif /* ZAPPY_NET_H */
