##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile for libzappy_net functional tests
##

NAME = functional_tests_zappy_net

CC = clang
CFLAGS = -Wall -Wextra -std=c11 -g3
LDFLAGS = -lcriterion -lpthread

# Directories
SRC_DIR = .
LIB_DIR = ../../../libzappy_net
OBJ_DIR = obj

# Library
LIB_NAME = $(LIB_DIR)/libzappy_net.so

# Source files
SRC_FILES = test_simple_functional.c \
            functional_test_helpers.c \
            test_functional_api_extensions.c

OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Include directories
INCLUDES = -I$(LIB_DIR)/include

.PHONY: all clean fclean re

all: $(LIB_NAME) $(NAME)

$(LIB_NAME):
	$(MAKE) -C $(LIB_DIR)

$(NAME): $(OBJ_FILES)
	$(CC) -o $@ $^ -L$(LIB_DIR) -lzappy_net $(LDFLAGS)
	@echo "$(NAME) created successfully!"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning functional test object files"
	$(RM) -r $(OBJ_DIR)

fclean: clean
	@echo "Cleaning functional test executable"
	$(RM) $(NAME)

re: fclean all

test: $(NAME)
	@echo "Running functional tests..."
	./$(NAME)

debug: CFLAGS += -DDEBUG
debug: re
