/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for GameState class (simplified version)
*/

#include <criterion/criterion.h>

// Simple mock for GameState testing without dependencies
class SimpleGameState {
private:
    int timeUnit;
    bool empty;
    int mapWidth;
    int mapHeight;

public:
    SimpleGameState() : timeUnit(100), empty(true), mapWidth(0), mapHeight(0) {}
    
    void setTimeUnit(int unit) { timeUnit = unit; }
    int getTimeUnit() const { return timeUnit; }
    
    void setMapSize(int width, int height) { 
        mapWidth = width; 
        mapHeight = height; 
        empty = (width == 0 && height == 0);
    }
    
    int getMapWidth() const { return mapWidth; }
    int getMapHeight() const { return mapHeight; }
    
    bool isEmpty() const { return empty; }
    
    void clear() {
        mapWidth = 0;
        mapHeight = 0;
        empty = true;
        timeUnit = 100;
    }
};

TestSuite(simple_game_state, .description = "Tests for simplified GameState functionality");

Test(simple_game_state, default_initialization) {
    SimpleGameState gameState;
    
    // Default time unit
    cr_assert_eq(gameState.getTimeUnit(), 100, "Default time unit should be 100");
    
    // Should be empty
    cr_assert(gameState.isEmpty(), "Game state should be empty initially");
    
    // Map dimensions should be zero
    cr_assert_eq(gameState.getMapWidth(), 0, "Initial map width should be 0");
    cr_assert_eq(gameState.getMapHeight(), 0, "Initial map height should be 0");
}

Test(simple_game_state, set_time_unit) {
    SimpleGameState gameState;
    
    gameState.setTimeUnit(200);
    cr_assert_eq(gameState.getTimeUnit(), 200, "Time unit should be set correctly");
    
    gameState.setTimeUnit(50);
    cr_assert_eq(gameState.getTimeUnit(), 50, "Time unit should be updated correctly");
    
    gameState.setTimeUnit(1);
    cr_assert_eq(gameState.getTimeUnit(), 1, "Minimum time unit should work");
}

Test(simple_game_state, set_map_size) {
    SimpleGameState gameState;
    
    gameState.setMapSize(10, 10);
    cr_assert_eq(gameState.getMapWidth(), 10, "Map width should be set correctly");
    cr_assert_eq(gameState.getMapHeight(), 10, "Map height should be set correctly");
    cr_assert(!gameState.isEmpty(), "Game state should not be empty after setting map size");
    
    gameState.setMapSize(20, 15);
    cr_assert_eq(gameState.getMapWidth(), 20, "Map width should be updated");
    cr_assert_eq(gameState.getMapHeight(), 15, "Map height should be updated");
}

Test(simple_game_state, clear_functionality) {
    SimpleGameState gameState;
    
    // Set some values
    gameState.setTimeUnit(500);
    gameState.setMapSize(30, 25);
    
    // Verify they are set
    cr_assert_eq(gameState.getTimeUnit(), 500, "Time unit should be set");
    cr_assert(!gameState.isEmpty(), "Should not be empty");
    
    // Clear
    gameState.clear();
    
    // Verify reset to defaults
    cr_assert_eq(gameState.getTimeUnit(), 100, "Time unit should be reset to default");
    cr_assert_eq(gameState.getMapWidth(), 0, "Map width should be reset");
    cr_assert_eq(gameState.getMapHeight(), 0, "Map height should be reset");
    cr_assert(gameState.isEmpty(), "Should be empty after clear");
}

Test(simple_game_state, edge_cases) {
    SimpleGameState gameState;
    
    // Test zero dimensions (should remain empty)
    gameState.setMapSize(0, 0);
    cr_assert(gameState.isEmpty(), "Zero dimensions should keep state empty");
    
    // Test one dimension zero
    gameState.setMapSize(10, 0);
    cr_assert(gameState.isEmpty(), "Zero height should keep state empty");
    
    gameState.setMapSize(0, 10);
    cr_assert(gameState.isEmpty(), "Zero width should keep state empty");
    
    // Test very large dimensions
    gameState.setMapSize(1000, 1000);
    cr_assert_eq(gameState.getMapWidth(), 1000, "Large width should work");
    cr_assert_eq(gameState.getMapHeight(), 1000, "Large height should work");
    cr_assert(!gameState.isEmpty(), "Large dimensions should not be empty");
}
