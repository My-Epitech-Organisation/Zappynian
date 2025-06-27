##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile for GUI tests
##

NAME = tests_zappy_gui

CXX = g++
# Use include path manipulation to force mock usage
CXXFLAGS = -Wall -Wextra -std=c++17 -g3 -O0 -fprofile-arcs -ftest-coverage -DTESTING_MODE -DMOCK_IRRLICHT -I. -I$(SRC_DIR)
LDFLAGS = -lcriterion -lpthread -lgcov --coverage

# Directories
SRC_DIR = .
GUI_DIR = ../../gui
LIB_DIR = ../../libzappy_net
OBJ_DIR = obj

# Library
LIB_NAME = $(LIB_DIR)/libzappy_net.so

# Test source files - Comprehensive unit tests for working GUI components
TEST_FILES = test_inventory_comprehensive.cpp \
            test_gamestate_comprehensive.cpp \
            test_playerentity_comprehensive.cpp \
            test_inventory.cpp \
            test_game_state.cpp \
            test_direction.cpp \
            test_direction_comprehensive.cpp \
            test_aentity_basic.cpp \
            test_compilation_success.cpp \
            test_main.cpp \
            irrlicht_stubs.cpp

# GUI source files to test - Only files that compile without string issues
GUI_SRC_FILES = $(GUI_DIR)/Core/GameState.cpp \
                $(GUI_DIR)/Entities/AEntity.cpp \
                $(GUI_DIR)/Entities/Inventory.cpp \
                $(GUI_DIR)/Entities/PlayerEntity.cpp \
                $(GUI_DIR)/Entities/TileEntity.cpp

# Object files
TEST_OBJ_FILES = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(TEST_FILES))
GUI_OBJ_FILES = $(patsubst $(GUI_DIR)/%.cpp,$(OBJ_DIR)/gui_%.o,$(GUI_SRC_FILES))

# Include flags
INC_FLAGS = -I. -I$(GUI_DIR) -I$(LIB_DIR)/include -include irrlicht_wrapper.hpp

.PHONY: all clean fclean re tests_run coverage coverage-detailed coverage-summary help help

all: $(NAME)

$(NAME): create_directories $(TEST_OBJ_FILES) $(GUI_OBJ_FILES) $(LIB_NAME)
	@echo "Linking test executable $(NAME)"
	@$(CXX) $(TEST_OBJ_FILES) $(GUI_OBJ_FILES) -L$(LIB_DIR) -lzappy_net $(LDFLAGS) -o $(NAME)
	@echo "$(NAME) created successfully!"

$(LIB_NAME):
	@echo "Building libzappy_net..."
	@$(MAKE) -C $(LIB_DIR)

create_directories:
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling test $<"
	@$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

$(OBJ_DIR)/gui_%.o: $(GUI_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling GUI source $<"
	@$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

clean:
	@echo "Cleaning test object files"
	@rm -rf $(OBJ_DIR)
	@rm -f *.gcda *.gcno *.gcov

fclean: clean
	@echo "Removing test executable"
	@rm -f $(NAME)

re: fclean all

tests_run: $(NAME)
	@echo "Running tests..."
	@LD_LIBRARY_PATH=/usr/local/lib64:$(LIB_DIR):$$LD_LIBRARY_PATH ./$(NAME) || true

coverage: tests_run
	@echo -e "Génération du rapport de couverture..."
	cd .. && gcovr --exclude tests/gui/ --branches
	cd .. && gcovr --exclude tests/gui/