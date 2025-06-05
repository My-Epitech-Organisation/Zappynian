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

# Debug and Sanitize options
DEBUG_FLAGS = -g3 -DDEBUG
SANITIZE_FLAGS = -fsanitize=address,undefined

# Include and source directories
INC_DIR = include
SRC_DIR = src

# Object directory
OBJ_DIR = obj

# Header files
HEADERS = $(INC_DIR)/libzappy_net.h

# Source files
SRC_C = $(SRC_DIR)/libzappy_net.c
SRC_CPP =

# All source files
SRC_FILES = $(SRC_C) $(SRC_CPP)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_C)) \
            $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_CPP))

# Include flags
INC_FLAGS = -I$(INC_DIR)

.PHONY: all clean fclean re debug sanitize

all: $(NAME)

# Create lib directory if it doesn't exist
$(NAME): create_directories $(OBJ_FILES)
	@echo "Creating library $(NAME)"
	@$(AR) $(ARFLAGS) $(NAME) $(OBJ_FILES)
	@echo "$(NAME) created successfully!"

create_directories:
	@mkdir -p $(OBJ_DIR)

# Compile C sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Compile C++ sources
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files"
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@echo "Cleaning library $(NAME)"
	@$(RM) $(NAME)

re: fclean all

debug: CFLAGS += $(DEBUG_FLAGS)
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: re

sanitize: CFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
sanitize: CXXFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
sanitize: re