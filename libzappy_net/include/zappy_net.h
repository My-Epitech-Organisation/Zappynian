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
    #include <poll.h>
    #include "zappy_net_error.h"

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
     * @brief Flush the socket's send buffer to the network
     *
     * This function attempts to send all data in the socket's send buffer
     * to the network. It may not send all data in a single call if the
     * network is not ready to accept more data.
     *
     * @param sock The socket handle
     * @return Number of bytes flushed, -1 on error with errno set
     */
    ssize_t zn_flush(zn_socket_t sock);

    /**
    * @brief Create a server socket and listen on specified port
    *
    * Creates a TCP server socket, binds it to the specified port on all
    * available interfaces, and starts listening for incoming connections.
    * The socket is created with O_NONBLOCK flag.
    *
    * @param port The port number to listen on (0-65535)
    *             Port 0 means auto-assign a free port
    * @return A valid socket handle on success, NULL on failure
    */
    zn_socket_t zn_server_listen(int port);

    /**
    * @brief Connect to a server as a client
    *
    * Creates a TCP client socket and attempts to connect to the specified
    * host and port. The socket is created with O_NONBLOCK flag.
    * The connection may not be immediately established due to
    * non-blocking nature.
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

    /**
    * @brief Get the file descriptor from a socket handle
    *
    * Retrieves the underlying file descriptor from a socket handle.
    * This is useful for advanced operations or integration with
    * other networking libraries.
    *
    * @param socket The socket handle
    * @return The file descriptor on success, -1 on failure
    */
    int zn_get_fd(zn_socket_t socket);

    /**
    * @brief Accept a connection on a server socket
    *
    * Accepts an incoming connection on a server socket and returns
    * a new socket handle for the accepted connection. The server socket
    * must be in listening state (created with zn_server_listen).
    * This function is non-blocking and will return NULL if no connection
    * is pending.
    *
    * @param server_sock The server socket handle (must be listening)
    * @param addr Pointer to sockaddr structure to store
    client address (optional)
    * @param len Pointer to socklen_t for
    address length (optional)
    * @return A new socket handle for the accepted connection, NULL on failure
    */
    zn_socket_t zn_accept(zn_socket_t server_sock,
        struct sockaddr *addr, socklen_t *len);

    /**
    * @brief Set or unset non-blocking mode on a socket
    *
    * Sets or removes the O_NONBLOCK flag on a socket's file descriptor.
    * When enabled, socket operations will not block and return immediately
    * even if they cannot complete. When disabled, operations may block
    * until they can complete.
    *
    * @param sock The socket handle to modify
    * @param enabled 1 to enable non-blocking mode, 0 to disable
    * @return 0 on success, -1 on failure
    */
    int zn_set_nonblocking(zn_socket_t sock, int enabled);

    /**
    * @brief Client role enumeration for handshake
    */
    typedef enum {
        ZN_ROLE_AI,         /**< AI client */
        ZN_ROLE_GUI,        /**< Graphical client */
        ZN_ROLE_UNKNOWN     /**< Unknown role */
    } zn_role_t;

    /**
    * @brief Handshake result structure
    */
    typedef struct {
        int client_num;     /**< Number of available slots */
        int world_x;        /**< World width */
        int world_y;        /**< World height */
    } zn_handshake_result_t;

    /**
    * @brief Perform client handshake with server
    *
    * Performs the complete Zappy handshake sequence for a client:
    * 1. Receives WELCOME message from server
    * 2. Sends team name or "GRAPHIC" for GUI
    * 3. Receives client number and world dimensions
    *
    * @param sock The connected client socket
    * @param role The client role (AI or GUI)
    * @param team_name The team name (ignored for GUI role)
    * @param result Pointer to structure to store handshake result
    * @return 0 on success, -1 on failure
    */
    int zn_perform_handshake(zn_socket_t sock, zn_role_t role,
        const char *team_name, zn_handshake_result_t *result);

    /**
    * @brief Send WELCOME message to client (server side)
    *
    * Sends the initial WELCOME message to a newly connected client.
    * This is the first step in the Zappy handshake protocol.
    *
    * @param sock The client socket
    * @return 0 on success, -1 on failure
    */
    int zn_send_welcome(zn_socket_t sock);

    /**
    * @brief Receive team name from client (server side)
    *
    * Receives and parses the team name sent by the client.
    * Returns the detected role based on the team name.
    *
    * @param sock The client socket
    * @param team_name Buffer to store team name (max 256 chars)
    * @param team_name_size Size of team_name buffer
    * @return Client role on success, ZN_ROLE_UNKNOWN on failure
    */
    zn_role_t zn_receive_team_name(zn_socket_t sock, char *team_name,
        size_t team_name_size);

    /**
    * @brief Send handshake response to client (server side)
    *
    * Sends the client number and world dimensions to complete
    * the handshake sequence.
    *
    * @param sock The client socket
    * @param client_num Number of available client slots
    * @param world_x World width
    * @param world_y World height
    * @return 0 on success, -1 on failure
    */
    int zn_send_handshake_response(zn_socket_t sock, int client_num,
        int world_x, int world_y);

    /**
    * @brief Send a message with automatic newline termination
    *
    * Sends a string message followed by a newline character to the socket.
    * This is a convenience function that combines zn_write() and zn_flush()
    * for sending complete text messages in the Zappy protocol.
    *
    * @param sock The socket handle
    * @param message The message string to send (null-terminated)
    * @return 0 on success, -1 on failure
    */
    int zn_send_message(zn_socket_t sock, const char *message);

    /**
    * @brief Check if socket has pending write data
    *
    * Checks if the socket's write buffer contains data that needs to be
    * sent to the network. This is useful for conditional POLLOUT monitoring
    * to avoid unnecessary polling on sockets with empty write buffers.
    *
    * @param sock The socket handle
    * @return 1 if has pending writes, 0 if empty or on error
    */
    int zn_has_pending_writes(zn_socket_t sock);

    /**
    * @brief Receive a complete message line from socket
    *
    * Receives a complete newline-terminated message from the socket.
    * This is a convenience function that uses zn_readline() and returns
    * a dynamically allocated string. The caller is responsible for freeing
    * the returned string.
    *
    * @param sock The socket handle
    * @return Dynamically allocated message string (caller must free),
    *         or NULL if no message available or on error
    */
    char *zn_receive_message(zn_socket_t sock);

    /**
    * @brief Maximum number of sockets supported by zn_poll()
    */
    #define ZN_POLL_MAX_SOCKETS 64

    /**
    * @brief Result structure for zn_poll() operations
    *
    * Contains bitmaps indicating which sockets are ready for various
    * operations. Each bit position corresponds to the index in the
    * sockets array passed to zn_poll().
    */
    typedef struct {
        uint64_t readable;      /**< Bitmap of sockets ready for reading */
        uint64_t writable;      /**< Bitmap of sockets ready for writing */
        uint64_t error;         /**< Bitmap of sockets with errors */
        int ready_count;        /**< Total number of ready sockets */
    } zn_poll_result_t;

    /**
    * @brief Poll multiple sockets with timeout
    *
    * Waits for events on multiple sockets with a specified timeout.
    * Returns a bitmap structure indicating which sockets are ready
    * for reading, writing, or have errors.
    *
    * @param sockets Array of socket handles to monitor
    * @param events Array of events to monitor for each socket
    * @param count Number of sockets in the array (max ZN_POLL_MAX_SOCKETS)
    * @param timeout_ms Timeout in milliseconds (-1 infinite, 0 no wait)
    * @return Poll result structure with bitmaps of ready sockets
    */
    zn_poll_result_t zn_poll(zn_socket_t *sockets, short *events,
        int count, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* ZAPPY_NET_H */
