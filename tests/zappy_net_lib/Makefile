##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile for libzappy_net tests
##

NAME = tests_zappy_net

CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g3
LDFLAGS = -lcriterion -lpthread

# Directories
SRC_DIR = .
LIB_DIR = ../../libzappy_net
OBJ_DIR = obj

# Library
LIB_NAME = $(LIB_DIR)/libzappy_net.a

# Source files
SRC_FILES = test_socket_creation.c \
            test_server_socket.c \
            test_client_socket.c \
            test_loopback.c \
            test_socket_close.c \
            test_edge_cases.c \
            test_pollbusy.c \
            test_helpers.c \
            test_ringbuf.c \
            test_readline_fragmentation.c \
            test_error_handling.c \
            test_result_api.c \
            test_handshake.c \
            test_simple_handshake.c

# Object files
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Include flags
INC_FLAGS = -I$(LIB_DIR)/include

.PHONY: all clean fclean re tests_run functional all_tests

all: $(NAME)

$(NAME): create_directories $(OBJ_FILES) $(LIB_NAME)
	@echo "Linking test executable $(NAME)"
	@$(CC) $(OBJ_FILES) $(LIB_NAME) $(LDFLAGS) -o $(NAME)
	@echo "$(NAME) created successfully!"

$(LIB_NAME):
	@echo "Building libzappy_net..."
	@$(MAKE) -C $(LIB_DIR)

create_directories:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	@echo "Cleaning test object files"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "Cleaning test executable $(NAME)"
	@rm -f $(NAME)

re: fclean all

tests_run: $(NAME)
	@echo "Running tests..."
	@./$(NAME) --verbose

functional:
	@echo "Running functional tests..."
	@$(MAKE) -C functional_tests test

all_tests: tests_run functional
	@echo "All tests completed!"
