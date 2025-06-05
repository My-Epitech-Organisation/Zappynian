##
## EPITECH PROJECT, 2025
## Zappynian
## File description:
## Makefile
##

.PHONY: all				\
		clean			\
		fclean re		\
        libzappy_net    \
        zappy_server	\
		zappy_gui		\
		zappy_ai

all: libzappy_net zappy_server zappy_gui zappy_ai

libzappy_net:
	@echo "Building Network Library..."
	$(MAKE) -C libzappy_net

zappy_server: libzappy_net
	@echo "Building Zappy Server..."
#	$(MAKE) -C server

zappy_gui: libzappy_net
	@echo "Building Zappy GUI..."
#	$(MAKE) -C gui

zappy_ai:
	@echo "Building Zappy AI..."
#	$(MAKE) -C ai

clean:
	$(MAKE) -C libzappy_net clean
	$(MAKE) -C server clean
	$(MAKE) -C gui clean
	$(MAKE) -C ai clean

fclean: clean
	$(MAKE) -C libzappy_net fclean
	$(MAKE) -C server fclean
	$(MAKE) -C gui fclean
	$(MAKE) -C ai fclean

re: fclean all

# Format check rule
check_normalize:
	@echo "Checking code format..."