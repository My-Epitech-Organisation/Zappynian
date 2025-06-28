##
## EPITECH PROJECT, 2025
## Zappynian
## File description:
## Makefile
##

NAME = zappy_gui

SRC = src/main.cpp \
	Event/EventReceiver.cpp \
	App/Game.cpp \
	Core/EntityManager.cpp \
	Core/WorldScene.cpp \
	Entities/AEntity.cpp \
	Entities/PlayerEntity.cpp \
	Entities/TileEntity.cpp \
	Entities/Inventory.cpp \
	Network/NetworkManager.cpp \
	Network/NetworkManagerImpl.cpp \
	Network/ProtocolParser.cpp \

OBJDIR = build
OBJ = $(SRC:%.cpp=$(OBJDIR)/%.o)

CXXFLAGS = -Wall -Wextra -std=c++17 -I./ -I../libzappy_net/include
LDFLAGS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -L../libzappy_net -lzappy_net\
		  -Wl,-rpath='$$ORIGIN/libzappy_net'

all: $(NAME)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	g++ $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
