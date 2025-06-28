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

SRCS	=	src/main.c							\
            src/display_help.c 					\
            src/display_infos.c 				\
            src/fill_n_c_f.c 					\
            src/fill_p_x_y.c 					\
			src/handle_free.c 					\
			src/parsing.c 						\
			src/set_server.c 					\
			src/error_handling.c 				\
			src/handle_client.c 				\
			src/client_manager.c 				\
			src/client_handshake.c 				\
			src/client_handshake_response.c 	\
			src/graphic_client_list.c 			\
			src/commands_movement.c				\
			src/commands_actions.c 				\
			src/commands_communication.c 		\
			src/commands_core.c 				\
			src/death.c 						\
			src/win.c 							\
			src/game_loop.c 					\
			src/resource.c 						\
			src/world.c 						\
			src/player.c 						\
			src/elevation.c 					\
			src/team.c 							\
			src/look_cmd.c 						\
			src/look_vision_tiles.c 			\
			src/look_current_tile.c 			\
			src/inventory_cmd.c 				\
			src/broadcast_cmd.c 				\
			src/network_integration.c 			\
			src/client_adapters.c 				\
			src/write_to_client.c 				\
			src/server_loop.c 					\
			src/server_poll.c 					\
			src/timing_manager.c 				\
			src/timing_integration.c 			\
			src/command_timer.c 				\
			src/food_timer.c 					\
			src/server_player_manager.c 		\
			src/server_event_handler.c 		\
			src/catch_commands_from_clients.c 	\
			src/egg.c 							\
			src/init_eggs.c 					\
			src/world_utils.c 					\
			src/eject_cmd.c 					\
			src/check_elevation.c 				\
			src/send_state_msg_to_players.c 	\
			src/send_init_gui.c 				\
			src/send_gui_egg_info.c 			\
			src/send_gui_player_info.c 			\
			src/send_gui_map_info.c 			\
			src/send_gui_team_info.c 			\
			src/send_gui_time_info.c 			\
			src/send_gui_broadcast_info.c 		\
			src/send_gui_resources_info.c 		\
			src/send_gui_status_info.c 			\
			src/send_gui_incantation_info.c 	\
			src/check_min_eggs.c 				\
			src/send_to_graphic_client.c 		\
			src/set_resources.c 				\

OBJS	=	$(SRCS:.c=.o)

TESTS	=	$(wildcard ../tests/server/*.c)

CFLAGS	= 	-I./includes -I../libzappy_net/include -Wall -Wextra -Werror

LIBZAPPY_NET_DIR = ../libzappy_net
LIBZAPPY_NET_LIB = $(LIBZAPPY_NET_DIR)/libzappy_net.so

LDFLAGS_PROD = -L$(LIBZAPPY_NET_DIR) -lzappy_net \
			   -Wl,-rpath='$$ORIGIN/libzappy_net'

CFLAGS_DEBUG = $(CFLAGS) -g
LDFLAGS_DEBUG = $(LDFLAGS_PROD) -g

CFLAGS_TEST = $(CFLAGS) --coverage
LDFLAGS_TEST = $(LDFLAGS_PROD) -lcriterion --coverage

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

debug: CFLAGS := $(CFLAGS_DEBUG)
debug: LDFLAGS_BUILD := $(LDFLAGS_DEBUG)
debug: build_libzappy_net $(NAME)
	@echo -e "$(GREEN)Compilation debug réussie, le binaire :$(ORANGE) \
	$(NAME) $(GREEN)générée avec symboles de debug$(NO_COLOR)"

build_libzappy_net:
	@echo -e "$(BLUE)Construction de libzappy_net...$(NO_COLOR)"
	$(MAKE) -C $(LIBZAPPY_NET_DIR)

%.o: %.c includes/server.h
	@echo -e "$(VIOLET)Compilation de $< en $@...$(NO_COLOR)"
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBZAPPY_NET_LIB)
	@echo -e "$(BLUE)Liaison des fichiers objets en $(NAME)...$(NO_COLOR)"
	$(CC) $(OBJS) -o $(NAME) $(or $(LDFLAGS_BUILD),$(LDFLAGS_PROD))

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
	$(CC) $(CFLAGS_TEST) $(filter-out src/main.c, $(SRCS)) $(TESTS) \
	-o ../$(NAME_TESTS) $(LDFLAGS_TEST)
	@echo -e "$(GREEN)Tests compilés: ./$(NAME_TESTS)$(NO_COLOR)"
	@echo "Exécution des tests..."
	../$(NAME_TESTS)

coverage : tests_run
	@echo -e "$(BLUE)Génération du rapport de couverture...$(NO_COLOR)"
	cd .. && gcovr --exclude tests/server/ --branches
	cd .. && gcovr --exclude tests/server/

.PHONY: all debug build_libzappy_net clean fclean re tests_run coverage
