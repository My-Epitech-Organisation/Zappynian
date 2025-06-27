/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Direction enum
*/

#include <criterion/criterion.h>
#include "Entities/Direction.hpp"

TestSuite(direction_comprehensive, .description = "Tests for Direction enum");

Test(direction_comprehensive, enum_values) {
    cr_assert_eq(static_cast<int>(Direction::NORTH), 0, "NORTH should be 0");
    cr_assert_eq(static_cast<int>(Direction::EAST), 1, "EAST should be 1");
    cr_assert_eq(static_cast<int>(Direction::SOUTH), 2, "SOUTH should be 2");
    cr_assert_eq(static_cast<int>(Direction::WEST), 3, "WEST should be 3");
}

Test(direction_comprehensive, enum_assignment) {
    Direction dir = Direction::NORTH;
    cr_assert_eq(dir, Direction::NORTH, "Direction assignment should work");
    
    dir = Direction::EAST;
    cr_assert_eq(dir, Direction::EAST, "Direction reassignment should work");
    
    dir = Direction::SOUTH;
    cr_assert_eq(dir, Direction::SOUTH, "Direction should be SOUTH");
    
    dir = Direction::WEST;
    cr_assert_eq(dir, Direction::WEST, "Direction should be WEST");
}

Test(direction_comprehensive, enum_comparison) {
    cr_assert(Direction::NORTH == Direction::NORTH, "Same directions should be equal");
    cr_assert(Direction::NORTH != Direction::EAST, "Different directions should not be equal");
    cr_assert(Direction::EAST != Direction::SOUTH, "EAST should not equal SOUTH");
    cr_assert(Direction::SOUTH != Direction::WEST, "SOUTH should not equal WEST");
}

Test(direction_comprehensive, enum_switch) {
    Direction dir = Direction::NORTH;
    int value = -1;
    
    switch (dir) {
        case Direction::NORTH:
            value = 0;
            break;
        case Direction::EAST:
            value = 1;
            break;
        case Direction::SOUTH:
            value = 2;
            break;
        case Direction::WEST:
            value = 3;
            break;
    }
    
    cr_assert_eq(value, 0, "Switch on NORTH should set value to 0");
    
    dir = Direction::SOUTH;
    switch (dir) {
        case Direction::NORTH:
            value = 0;
            break;
        case Direction::EAST:
            value = 1;
            break;
        case Direction::SOUTH:
            value = 2;
            break;
        case Direction::WEST:
            value = 3;
            break;
    }
    
    cr_assert_eq(value, 2, "Switch on SOUTH should set value to 2");
}
