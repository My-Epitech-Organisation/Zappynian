CXXFLAGS = -Wall -Wextra -std=c++17 -I../../gui -I../../gui/libzappy_net/include
LDFLAGS = -lgtest -lpthread
SRC_GUI = ../../gui

# Entities
ENTITIES_OBJ = $(SRC_GUI)/Entities/PlayerEntity.cpp $(SRC_GUI)/Entities/AEntity.cpp $(SRC_GUI)/Entities/TileEntity.cpp $(SRC_GUI)/Entities/Inventory.cpp
# NetworkClient
NETWORK_OBJ = $(SRC_GUI)/Network/NetworkClient.cpp $(SRC_GUI)/Core/WorldScene.cpp $(SRC_GUI)/Core/EntityManager.cpp $(SRC_GUI)/Entities/PlayerEntity.cpp $(SRC_GUI)/Entities/AEntity.cpp $(SRC_GUI)/Entities/TileEntity.cpp $(SRC_GUI)/Entities/Inventory.cpp $(SRC_GUI)/Event/EventReceiver.cpp
# WorldScene
WORLD_OBJ = $(SRC_GUI)/Core/WorldScene.cpp $(SRC_GUI)/Core/EntityManager.cpp $(SRC_GUI)/Entities/PlayerEntity.cpp $(SRC_GUI)/Entities/AEntity.cpp $(SRC_GUI)/Entities/TileEntity.cpp $(SRC_GUI)/Entities/Inventory.cpp $(SRC_GUI)/Event/EventReceiver.cpp

all: test_Entities test_NetworkClient test_WorldScene

# Entities
./test_Entities: test_Entities.cpp $(ENTITIES_OBJ)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# NetworkClient
./test_NetworkClient: test_NetworkClient.cpp $(NETWORK_OBJ)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# WorldScene
./test_WorldScene: test_WorldScene.cpp $(WORLD_OBJ)
	g++ $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

run:
	./test_Entities && ./test_NetworkClient && ./test_WorldScene

clean:
	rm -f test_Entities test_NetworkClient test_WorldScene *.gcda *.gcno

coverage: CXXFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: clean all run

.PHONY: all clean run coverage
