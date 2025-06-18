##
## EPITECH PROJECT, 2025
## Zappynian
## File description:
## Makefile
##

NAME		=	zappy_server
NAME_TESTS	=	unit_tests

CC		=	gcc
RM		=	rm -f

SRCS	=	src/main.c				\
            src/display_help.c 		\
            src/display_infos.c 	\
            src/fill_n_c_f.c 		\
            src/fill_p_x_y.c 		\
			src/handle_free.c 		\
			src/parsing.c 			\
			src/set_server.c 		\
			src/error_handling.c 	\
			src/handle_client.c 	\
			src/client_manager.c 	\
			src/commands_movement.c	\
			src/commands_actions.c 	\
			src/commands_communication.c \
			src/commands_core.c 	\
			src/death.c 			\
			src/win.c 				\
			src/game_loop.c 		\
			src/resource.c 			\
			src/world.c 			\
			src/player.c 			\
			src/elevation.c 		\
			src/team.c 				\
			src/network_integration.c \
			src/client_adapters.c 	\
			src/write_to_client.c 	\
			src/server_loop.c 		\

OBJS	=	$(SRCS:.c=.o)

TESTS	=	$(wildcard ../tests/server/*.c)

CFLAGS	= 	-I./includes -I../libzappy_net/include -Wall -Wextra -Werror

# Bibliothèque libzappy_net
LIBZAPPY_NET_DIR = ../libzappy_net
LIBZAPPY_NET_LIB = $(LIBZAPPY_NET_DIR)/libzappy_net.so

# LDFLAGS avec RPATH pour hardcoder le chemin de la bibliothèque
LDFLAGS	=  -L$(LIBZAPPY_NET_DIR) -lzappy_net -lcriterion --coverage \
		   -Wl,-rpath='$$ORIGIN/libzappy_net'

NO_COLOR=\033[0m
RED=\033[0;31m
GREEN=\033[0;32m
BLUE=\033[0;34m
ORANGE=\033[0;33m
VIOLET=\033[0;35m
ROSE=\033[0;36m

all: build_libzappy_net $(NAME)
	@echo -e "$(GREEN)Compilation réussie, le binaire :$(ORANGE) $(NAME) \
	$(GREEN)générée $(NO_COLOR)"

build_libzappy_net:
	@echo -e "$(BLUE)Construction de libzappy_net...$(NO_COLOR)"
	$(MAKE) -C $(LIBZAPPY_NET_DIR)

%.o: %.c includes/server.h
	@echo -e "$(VIOLET)Compilation de $< en $@...$(NO_COLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBZAPPY_NET_LIB)
	@echo -e "$(BLUE)Liaison des fichiers objets en $(NAME)...$(NO_COLOR)"
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo -e "$(GREEN)Copie de l'exécutable vers la racine du projet...$(NO_COLOR)"
	cp $(NAME) ../$(NAME)

clean:
	@echo -e "$(RED)Suppression des fichiers objets (.o)...$(NO_COLOR)"
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBZAPPY_NET_DIR) clean

fclean: clean
	@echo -e "$(RED)Suppression des tests...$(NO_COLOR)"
	@echo -e "$(RED)Suppression des fichiers de couverture...$(NO_COLOR)"
	$(RM) -f ../$(NAME_TESTS)
	$(RM) -f $(NAME)
	$(RM) -f ../*.gcda
	$(RM) -f ../*.gcno
	$(MAKE) -C $(LIBZAPPY_NET_DIR) fclean

re: fclean all

tests_run:
	@echo "Running tests..."
	@echo -e "$(BLUE)Compilation des tests...$(NO_COLOR)"
	$(CC) $(CFLAGS) $(filter-out src/main.c, $(SRCS)) $(TESTS) \
	-o ../$(NAME_TESTS) $(LDFLAGS)
	@echo -e "$(GREEN)Tests compilés: ./$(NAME_TESTS)$(NO_COLOR)"
	@echo "Exécution des tests..."
	../$(NAME_TESTS)

coverage : tests_run
	@echo -e "$(BLUE)Génération du rapport de couverture...$(NO_COLOR)"
	cd .. && gcovr --exclude tests/server/ --branches
	cd .. && gcovr --exclude tests/server/

.PHONY: all build_libzappy_net clean fclean re tests_run coverage
