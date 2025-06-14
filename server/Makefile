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

OBJS	=	$(SRCS:.c=.o)

TESTS	=	$(wildcard ../tests/server/*.c)

CFLAGS	= 	-I./includes -Wall -Wextra -Werror
LDFLAGS	=  -lcriterion --coverage

NO_COLOR=\033[0m
RED=\033[0;31m
GREEN=\033[0;32m
BLUE=\033[0;34m
ORANGE=\033[0;33m
VIOLET=\033[0;35m
ROSE=\033[0;36m

all: $(NAME)
	@echo -e "$(GREEN)Compilation réussie, le binaire :$(ORANGE) $(NAME) \
	$(GREEN)générée $(NO_COLOR)"

%.o: %.c includes/server.h
	@echo -e "$(VIOLET)Compilation de $< en $@...$(NO_COLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@echo -e "$(BLUE)Liaison des fichiers objets en $(NAME)...$(NO_COLOR)"
	$(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	@echo -e "$(RED)Suppression des fichiers objets (.o)...$(NO_COLOR)"
	$(RM) $(OBJS)

fclean: clean
	@echo -e "$(RED)Suppression des tests...$(NO_COLOR)"
	@echo -e "$(RED)Suppression des fichiers de couverture...$(NO_COLOR)"
	$(RM) -f ../$(NAME_TESTS)
	$(RM) -f ../*.gcda
	$(RM) -f ../*.gcno

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

.PHONY: all clean fclean re tests_run coverage
