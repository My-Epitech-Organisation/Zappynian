##
## EPITECH PROJECT, 2025
## Zappynian
## File description:
## Makefile
##

.PHONY: all clean re

NAME = zappy_ai

CP = cp -f
RM = rm -f

SRC = src/zappy_ai.py

zappy_ai: $(SRC)
	@echo "Building Zappy AI..."
	@$(CP) $(SRC) ./zappy_ai
	@chmod +x ./zappy_ai
	@echo "Zappy AI built successfully!"

all: $(NAME)

clean:
	$(RM) -f ./zappy_ai

fclean: clean

re: clean all
