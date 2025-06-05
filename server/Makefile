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

SRCS	=	$(wildcard src/*.c)
OBJS	=	$(SRCS:.c=.o)

TESTS	=	$(wildcard ../tests/server/*.c)

CFLAGS	=	-I./include -Wall -Wextra -Werror
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
	@echo -e "$(RED)Suppression de $(NAME)...$(NO_COLOR)"
	$(RM) ../$(NAME)
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
	@echo -e "$(GREEN)Tests compilés: tests/$(NAME_TESTS)$(NO_COLOR)"
	@echo "Exécution des tests..."
	../$(NAME_TESTS)

coverage : tests_run
	@echo -e "$(BLUE)Génération du rapport de couverture...$(NO_COLOR)"
	cd .. && gcovr --exclude tests/server/ branch
	cd .. && gcovr --exclude tests/server/

.PHONY: all clean fclean re tests_run coverage
