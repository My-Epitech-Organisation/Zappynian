##
## EPITECH PROJECT, 2025
## Zappynian
## File description:
## Makefile
##

.PHONY: all				\
		clean			\
		fclean re		\
        zappy_server	\
		zappy_gui		\
		zappy_ai

all: zappy_server zappy_gui zappy_ai

zappy_server:
	@echo "Building Zappy Server..."
#	$(MAKE) -C server

zappy_gui:
	@echo "Building Zappy GUI..."
#	$(MAKE) -C gui

zappy_ai:
	@echo "Building Zappy AI..."
#	$(MAKE) -C ai

clean:
	$(MAKE) -C common clean
	$(MAKE) -C server clean
	$(MAKE) -C gui clean
	$(MAKE) -C ai clean

fclean: clean
	$(MAKE) -C server fclean
	$(MAKE) -C gui fclean
	$(MAKE) -C ai fclean

re: fclean all

# Format check rule
check_normalize:
	@echo "Checking code format..."