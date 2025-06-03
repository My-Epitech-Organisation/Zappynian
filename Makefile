.PHONY: all clean fclean re \
        zappy_server zappy_gui zappy_ai

all: zappy_server zappy_gui zappy_ai

zappy_server:
	$(MAKE) -C server

zappy_gui:
	$(MAKE) -C gui

zappy_ai:
	$(MAKE) -C ai

clean:
	$(MAKE) -C common clean
	$(MAKE) -C server clean
	$(MAKE) -C gui clean
	# ai clean si besoin

fclean: clean
	$(MAKE) -C server fclean
	$(MAKE) -C gui fclean
	# ai fclean si besoin

re: fclean all
