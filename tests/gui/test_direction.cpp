/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Direction enum
*/

#include <criterion/criterion.h>
#include "Entities/Direction.hpp"

TestSuite(direction);

Test(direction, enum_values) {
    // Test that all direction values exist and are distinct
    Direction north = Direction::NORTH;
    Direction east = Direction::EAST;
    Direction south = Direction::SOUTH;
    Direction west = Direction::WEST;
    
    cr_assert_neq(north, east);
    cr_assert_neq(north, south);
    cr_assert_neq(north, west);
    cr_assert_neq(east, south);
    cr_assert_neq(east, west);
    cr_assert_neq(south, west);
}

Test(direction, enum_casting) {
    // Test casting to int (useful for rotations)
    cr_assert_eq(static_cast<int>(Direction::NORTH), 0);
    cr_assert_eq(static_cast<int>(Direction::EAST), 1);
    cr_assert_eq(static_cast<int>(Direction::SOUTH), 2);
    cr_assert_eq(static_cast<int>(Direction::WEST), 3);
}

Test(direction, comparison_operations) {
    Direction dir1 = Direction::NORTH;
    Direction dir2 = Direction::NORTH;
    Direction dir3 = Direction::EAST;
    
    // Test equality
    cr_assert_eq(dir1, dir2);
    cr_assert_neq(dir1, dir3);
    
    // Test assignment
    dir1 = Direction::SOUTH;
    cr_assert_eq(dir1, Direction::SOUTH);
    cr_assert_neq(dir1, dir2);
}

Test(direction, switch_statement_coverage) {
    // Test that all directions can be used in switch statements
    Direction directions[] = {Direction::NORTH, Direction::EAST, Direction::SOUTH, Direction::WEST};
    int results[4];
    
    for (int i = 0; i < 4; ++i) {
        switch (directions[i]) {
            case Direction::NORTH:
                results[i] = 0;
                break;
            case Direction::EAST:
                results[i] = 1;
                break;
            case Direction::SOUTH:
                results[i] = 2;
                break;
            case Direction::WEST:
                results[i] = 3;
                break;
        }
    }
    
    cr_assert_eq(results[0], 0);
    cr_assert_eq(results[1], 1);
    cr_assert_eq(results[2], 2);
    cr_assert_eq(results[3], 3);
}

Test(direction, rotation_math) {
    // Test rotation calculations commonly used with directions
    Direction current = Direction::NORTH;
    
    // Clockwise rotation
    Direction clockwise = static_cast<Direction>((static_cast<int>(current) + 1) % 4);
    cr_assert_eq(clockwise, Direction::EAST);
    
    current = Direction::WEST;
    clockwise = static_cast<Direction>((static_cast<int>(current) + 1) % 4);
    cr_assert_eq(clockwise, Direction::NORTH);
    
    // Counter-clockwise rotation
    current = Direction::NORTH;
    Direction counterClockwise = static_cast<Direction>((static_cast<int>(current) + 3) % 4);
    cr_assert_eq(counterClockwise, Direction::WEST);
    
    current = Direction::EAST;
    counterClockwise = static_cast<Direction>((static_cast<int>(current) + 3) % 4);
    cr_assert_eq(counterClockwise, Direction::NORTH);
}

Test(direction, angle_conversion) {
    // Test angle conversion (useful for 3D rotations)
    
    // NORTH = 0 degrees
    float angle = static_cast<float>(Direction::NORTH) * 90.0f;
    cr_assert_float_eq(angle, 0.0f, 0.001f);
    
    // EAST = 90 degrees
    angle = static_cast<float>(Direction::EAST) * 90.0f;
    cr_assert_float_eq(angle, 90.0f, 0.001f);
    
    // SOUTH = 180 degrees
    angle = static_cast<float>(Direction::SOUTH) * 90.0f;
    cr_assert_float_eq(angle, 180.0f, 0.001f);
    
    // WEST = 270 degrees
    angle = static_cast<float>(Direction::WEST) * 90.0f;
    cr_assert_float_eq(angle, 270.0f, 0.001f);
}

Test(direction, opposite_direction) {
    // Test finding opposite directions
    Direction north = Direction::NORTH;
    Direction opposite = static_cast<Direction>((static_cast<int>(north) + 2) % 4);
    cr_assert_eq(opposite, Direction::SOUTH);
    
    Direction east = Direction::EAST;
    opposite = static_cast<Direction>((static_cast<int>(east) + 2) % 4);
    cr_assert_eq(opposite, Direction::WEST);
    
    Direction south = Direction::SOUTH;
    opposite = static_cast<Direction>((static_cast<int>(south) + 2) % 4);
    cr_assert_eq(opposite, Direction::NORTH);
    
    Direction west = Direction::WEST;
    opposite = static_cast<Direction>((static_cast<int>(west) + 2) % 4);
    cr_assert_eq(opposite, Direction::EAST);
}

Test(direction, array_indexing) {
    // Test using directions as array indices
    const char* directionNames[] = {"North", "East", "South", "West"};
    
    cr_assert_str_eq(directionNames[static_cast<int>(Direction::NORTH)], "North");
    cr_assert_str_eq(directionNames[static_cast<int>(Direction::EAST)], "East");
    cr_assert_str_eq(directionNames[static_cast<int>(Direction::SOUTH)], "South");
    cr_assert_str_eq(directionNames[static_cast<int>(Direction::WEST)], "West");
}

Test(direction, vector_movement) {
    // Test direction to movement vector conversion
    int dx[] = {0, 1, 0, -1};  // NORTH, EAST, SOUTH, WEST
    int dy[] = {-1, 0, 1, 0};  // NORTH, EAST, SOUTH, WEST
    
    Direction dir = Direction::NORTH;
    cr_assert_eq(dx[static_cast<int>(dir)], 0);
    cr_assert_eq(dy[static_cast<int>(dir)], -1);
    
    dir = Direction::EAST;
    cr_assert_eq(dx[static_cast<int>(dir)], 1);
    cr_assert_eq(dy[static_cast<int>(dir)], 0);
    
    dir = Direction::SOUTH;
    cr_assert_eq(dx[static_cast<int>(dir)], 0);
    cr_assert_eq(dy[static_cast<int>(dir)], 1);
    
    dir = Direction::WEST;
    cr_assert_eq(dx[static_cast<int>(dir)], -1);
    cr_assert_eq(dy[static_cast<int>(dir)], 0);
}
