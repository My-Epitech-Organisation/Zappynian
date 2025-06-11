##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile for libzappy_net
##

NAME = libzappy_net.a

CC = clang
CXX = clang++
AR = ar
ARFLAGS = rcs
RM = rm -f

CFLAGS = -Wall -Wextra -pedantic -std=c11 -O2
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O2

DEBUG_FLAGS = -g3 -DDEBUG
SANITIZE_FLAGS = -fsanitize=address,undefined

INC_DIR = include
SRC_DIR = src

OBJ_DIR = obj

HEADERS = $(INC_DIR)/libzappy_net.h \
          $(INC_DIR)/zappy_net.h \
          $(INC_DIR)/zappy_net_ringbuf.h

SRC_C = $(SRC_DIR)/libzappy_net.c \
        $(SRC_DIR)/zappy_net_core.c \
        $(SRC_DIR)/zappy_net_socket.c \
        $(SRC_DIR)/zappy_net_server.c \
        $(SRC_DIR)/zappy_net_client.c \
        $(SRC_DIR)/zappy_net_poll.c \
        $(SRC_DIR)/zappy_net_ringbuf_core.c \
        $(SRC_DIR)/zappy_net_ringbuf_utils.c \
        $(SRC_DIR)/zappy_net_ringbuf_check.c \
        $(SRC_DIR)/zappy_net_ringbuf_write.c \
        $(SRC_DIR)/zappy_net_ringbuf_read.c \
        $(SRC_DIR)/zappy_net_ringbuf_io.c \
        $(SRC_DIR)/zappy_net_io.c
SRC_CPP =

SRC_FILES = $(SRC_C) $(SRC_CPP)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_C)) \
            $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP))

INC_FLAGS = -I$(INC_DIR)

.PHONY: all clean fclean re debug sanitize tests_run create_directories

all: $(NAME)

$(NAME): create_directories $(OBJ_FILES)
	@echo "Creating library $(NAME)"
	@$(AR) $(ARFLAGS) $(NAME) $(OBJ_FILES)
	@echo "$(NAME) created successfully!"

create_directories:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files"
	@$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C ../tests/zappy_net_lib clean

fclean: clean
	@echo "Cleaning library $(NAME)"
	@$(RM) $(NAME)
	@$(MAKE) -C ../tests/zappy_net_lib fclean

re: fclean all

debug: CFLAGS += $(DEBUG_FLAGS)
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: re

sanitize: CFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
sanitize: CXXFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
sanitize: re

tests_run: $(NAME)
	@echo "Running libzappy_net tests..."
	@$(MAKE) -C ../tests/zappy_net_lib tests_run
