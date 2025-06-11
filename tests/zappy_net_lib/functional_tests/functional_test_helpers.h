/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Functional test helpers for libzappy_net
*/

#ifndef FUNCTIONAL_TEST_HELPERS_H
#define FUNCTIONAL_TEST_HELPERS_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L

#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#include "zappy_net_ringbuf.h"

/* Test configuration */
#define TEST_SERVER_PORT 8080
#define TEST_CLIENT_COUNT 10
#define TEST_MESSAGE_SIZE 1024
#define TEST_TIMEOUT_MS 5000
#define TEST_STRESS_ITERATIONS 1000

/* Test message structures */
typedef struct {
    int id;
    char data[256];
    size_t size;
} test_message_t;

typedef struct {
    int port;
    int client_count;
    int message_count;
    double timeout_sec;
} test_config_t;

/* Helper function prototypes */
int create_test_server(int port);
int create_test_client(const char *host, int port);
void cleanup_test_environment(void);
int send_test_message(int socket, const test_message_t *msg);
int receive_test_message(int socket, test_message_t *msg);
void generate_random_message(test_message_t *msg, int id);
double get_time_diff_ms(struct timespec start, struct timespec end);
int wait_for_connection(int server_socket, double timeout_sec);
void setup_test_environment(void);
void teardown_test_environment(void);

/* Performance measurement helpers */
typedef struct {
    double min_latency;
    double max_latency;
    double avg_latency;
    double total_time;
    int success_count;
    int error_count;
} performance_stats_t;

void init_performance_stats(performance_stats_t *stats);
void update_performance_stats(performance_stats_t *stats, double latency);
void finalize_performance_stats(performance_stats_t *stats);
void print_performance_stats(const performance_stats_t *stats,
    const char *test_name);

/* Stress test helpers */
int run_stress_test_client(int port, int client_id, int message_count);
int run_stress_test_server(int port, int expected_clients);

/* Protocol test helpers */
int test_protocol_handshake(int client_socket, int server_socket);
int test_protocol_message_exchange(int client_socket, int server_socket);
int test_protocol_disconnection(int client_socket, int server_socket);

#endif /* FUNCTIONAL_TEST_HELPERS_H */
