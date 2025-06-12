##
## EPITECH PROJECT, 2025
## Zappynian
## File description:
## Makefile
##

.PHONY: all				\
		clean			\
		fclean re		\
		tests_run		\
		coverage		\
        libzappy_net    \
        zappy_server	\
		zappy_gui		\
		zappy_ai

MV = mv -f

RM = rm -f

CP = cp -f

all: libzappy_net zappy_server zappy_gui zappy_ai

libzappy_net:
	@echo "Building Network Library..."
	$(MAKE) -C libzappy_net

zappy_server: libzappy_net
	@echo "Building Zappy Server..."
	$(MAKE) -C server zappy_server
	$(MV) server/zappy_server .

zappy_gui: libzappy_net
	@echo "Building Zappy GUI..."
	$(MAKE) -C gui zappy_gui
	$(MV) gui/zappy_gui .

zappy_ai:
	@echo "Building Zappy AI..."
	$(MAKE) -C ai zappy_ai
	$(MV) ai/zappy_ai .

clean:
	$(MAKE) -C libzappy_net clean
	$(MAKE) -C server clean
	$(MAKE) -C gui clean
	$(MAKE) -C ai clean

fclean_libzappy_net:
	$(MAKE) -C libzappy_net fclean

fclean_zappy_server:
	$(MAKE) -C server fclean
	$(RM) -f zappy_server

fclean_zappy_gui:
	$(MAKE) -C gui fclean
	$(RM) -f zappy_gui

fclean_zappy_ai:
	$(MAKE) -C ai fclean
	$(RM) -f zappy_ai

fclean: clean fclean_libzappy_net fclean_zappy_server fclean_zappy_gui \
	fclean_zappy_ai
	@echo "Removing all generated files..."

re: fclean all

tests_run:
	$(MAKE) -C server tests_run

coverage:
	$(MAKE) -C server coverage

# Format check rule
check_normalize:
	@echo "Checking code format..."
