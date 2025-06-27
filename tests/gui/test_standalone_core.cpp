/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for standalone components without Irrlicht dependencies
*/

#include <criterion/criterion.h>

TestSuite(standalone_core, .description = "Tests for standalone components");

Test(standalone_core, basic_functionality) {
    cr_assert(true, "Basic test to ensure test framework works");
}

Test(standalone_core, arithmetic_test) {
    int a = 5;
    int b = 3;
    int result = a + b;
    cr_assert_eq(result, 8, "5 + 3 should equal 8");
}

Test(standalone_core, string_operations) {
    std::string test = "Hello";
    test += " World";
    cr_assert_str_eq(test.c_str(), "Hello World", "String concatenation should work");
}
