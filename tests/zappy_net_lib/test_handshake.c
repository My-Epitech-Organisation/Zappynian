/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test for Zappy handshake functionality
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "zappy_net.h"
#include "test_helpers.h"

static void *mock_server_thread(void *arg);
static void mock_server_ai_handshake(int client_fd);
static void mock_server_gui_handshake(int client_fd);

typedef struct {
    int port;
    int is_gui;
} server_thread_data_t;

TestSuite(handshake, .init = cr_redirect_stderr);

Test(handshake, ai_client_handshake_validation)
{
    zn_handshake_result_t result = {0};

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    
    // Test avec des paramètres invalides
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_AI, "team1", &result),
        -1, "Should fail with NULL socket");
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_AI, NULL, &result),
        -1, "Should fail with NULL team for AI");
    
    zn_cleanup();
}

Test(handshake, gui_client_handshake_validation)
{
    zn_handshake_result_t result = {0};

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    
    // Pour GUI, team_name peut être NULL
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_GUI, NULL, &result),
        -1, "Should fail with NULL socket even for GUI");
    
    zn_cleanup();
}

Test(handshake, invalid_parameters)
{
    zn_handshake_result_t result = {0};

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_AI, "team1", &result),
        -1, "Should fail with NULL socket");
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_AI, "team1", NULL),
        -1, "Should fail with NULL result");
    zn_cleanup();
}

Test(handshake, server_side_functions)
{
    zn_socket_t server_sock = NULL;
    char team_name[256] = {0};
    zn_role_t role = ZN_ROLE_UNKNOWN;

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    server_sock = zn_socket_create();
    cr_assert_not_null(server_sock, "Socket creation failed");
    cr_assert_eq(zn_send_welcome(NULL), -1,
        "Should fail with NULL socket");
    role = zn_receive_team_name(NULL, team_name, sizeof(team_name));
    cr_assert_eq(role, ZN_ROLE_UNKNOWN,
        "Should return UNKNOWN with NULL socket");
    cr_assert_eq(zn_send_handshake_response(NULL, 5, 10, 10), -1,
        "Should fail with NULL socket");
    zn_socket_destroy(server_sock);
    zn_cleanup();
}

static void *mock_server_thread(void *arg)
{
    server_thread_data_t *data = (server_thread_data_t *)arg;
    int server_fd = 0;
    int client_fd = 0;
    struct sockaddr_in addr = {0};
    socklen_t addr_len = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        return NULL;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(data->port);
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(server_fd);
        return NULL;
    }
    if (listen(server_fd, 1) == -1) {
        close(server_fd);
        return NULL;
    }
    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
    if (client_fd != -1) {
        if (data->is_gui) {
            mock_server_gui_handshake(client_fd);
        } else {
            mock_server_ai_handshake(client_fd);
        }
        close(client_fd);
    }
    close(server_fd);
    return NULL;
}

static void mock_server_ai_handshake(int client_fd)
{
    char buffer[256] = {0};
    ssize_t bytes_read = 0;

    if (send(client_fd, "WELCOME\n", 8, 0) == -1) {
        return;
    }
    bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        return;
    }
    buffer[bytes_read] = '\0';
    if (send(client_fd, "5\n", 2, 0) == -1) {
        return;
    }
    if (send(client_fd, "10 10\n", 6, 0) == -1) {
        return;
    }
}

static void mock_server_gui_handshake(int client_fd)
{
    char buffer[256] = {0};
    ssize_t bytes_read = 0;

    if (send(client_fd, "WELCOME\n", 8, 0) == -1) {
        return;
    }
    bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0) {
        return;
    }
    buffer[bytes_read] = '\0';
}

Test(handshake, send_welcome_only)
{
    zn_socket_t mock_sock = NULL;

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    mock_sock = zn_socket_create();
    cr_assert_not_null(mock_sock, "Socket creation failed");
    cr_assert_eq(zn_send_welcome(mock_sock), -1,
        "Should fail on unconnected socket");
    zn_socket_destroy(mock_sock);
    zn_cleanup();
}

Test(handshake, api_validation_only)
{
    zn_handshake_result_t result = {0};
    char team_name[256] = {0};
    zn_role_t role = ZN_ROLE_UNKNOWN;

    cr_assert_eq(zn_init(), 0, "Library initialization failed");
    
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_AI, "team", &result), -1,
        "Should fail with NULL socket");
    cr_assert_eq(zn_perform_handshake(NULL, ZN_ROLE_AI, NULL, &result), -1,
        "Should fail with NULL team_name for AI");
    
    cr_assert_eq(zn_send_welcome(NULL), -1, "Should fail with NULL socket");
    
    role = zn_receive_team_name(NULL, team_name, sizeof(team_name));
    cr_assert_eq(role, ZN_ROLE_UNKNOWN, "Should return UNKNOWN for NULL socket");
    
    cr_assert_eq(zn_send_handshake_response(NULL, 5, 10, 10), -1,
        "Should fail with NULL socket");
    
    zn_cleanup();
}