NAME = zappy_gui

SRC = src/main.cpp

OBJDIR = build
OBJ = $(SRC:src/%.cpp=$(OBJDIR)/%.o)

CXXFLAGS = -I./include -I../include
LDFLAGS = -lIrrlicht -lGL -lXxf86vm -lXext -lX11

all: $(NAME)

$(OBJDIR)/%.o: src/%.cpp
	mkdir -p $(OBJDIR)
	g++ $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
