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
     * @brief Write data to the socket's send buffer
     *
     * This function writes data to the socket's internal send buffer.
     * The data is not immediately sent over the network but queued for
     * later transmission when zn_flush() is called.
     *
     * @param sock The socket handle
     * @param data Pointer to data to write
     * @param len Length of data in bytes
     * @return Number of bytes written, -1 on error with errno set
     */
    ssize_t zn_write(zn_socket_t sock, const void *data, size_t len);

    /**
     * @brief Read data from the socket's receive buffer
     *
     * This function reads data from the socket's internal receive buffer.
     * If the buffer is empty, it attempts to read from the socket into
     * the buffer first. This function does not block if no data is available.
     *
     * @param sock The socket handle
     * @param data Buffer to store read data
     * @param len Maximum amount of data to read
     * @return Number of bytes read, -1 on error with errno set, 0 if no data
     */
    ssize_t zn_read(zn_socket_t sock, void *data, size_t len);

    /**
     * @brief Read a line from the socket's receive buffer
     *
     * This function reads a complete line (ending with '\n') from the socket's
     * internal receive buffer. If no complete line is available, it attempts
     * to read from the socket into the buffer first.
     *
     * @param sock The socket handle
     * @param data Buffer to store read data
     * @param len Maximum amount of data to read
     * @return Number of bytes read (including newline), -1 on error or no line
     */
    ssize_t zn_readln(zn_socket_t sock, void *data, size_t len);

    /**
     * @brief Read a complete line from socket with partial segment support
     *
     * This function reads a complete newline-terminated line from the socket.
     * It handles TCP fragmentation by accumulating partial segments until
     * a complete line is available. Returns dynamically allocated string
     * containing the complete line (without newline) or NULL if no complete
     * line is available.
     *
     * @param sock The socket handle
     * @return Dynamically allocated string with complete line (caller must
     *         free), or NULL if no complete line available or on error
     */
    char *zn_readline(zn_socket_t sock);

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
