/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive unit tests for Inventory class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../gui/Entities/Inventory.hpp"

TestSuite(Inventory, .timeout = 5);

Test(Inventory, default_construction) {
    Inventory inventory;
    
    // Test that default items are initialized to 0
    cr_assert_eq(inventory.getItemQuantity("food"), 0);
    cr_assert_eq(inventory.getItemQuantity("linemate"), 0);
    cr_assert_eq(inventory.getItemQuantity("deraumere"), 0);
    cr_assert_eq(inventory.getItemQuantity("sibur"), 0);
    cr_assert_eq(inventory.getItemQuantity("mendiane"), 0);
    cr_assert_eq(inventory.getItemQuantity("phiras"), 0);
    cr_assert_eq(inventory.getItemQuantity("thystame"), 0);
    cr_assert_eq(inventory.getItemQuantity("egg"), 0);
}

Test(Inventory, add_single_item) {
    Inventory inventory;
    
    inventory.addItem("food", 5);
    cr_assert_eq(inventory.getItemQuantity("food"), 5);
}

Test(Inventory, add_multiple_items) {
    Inventory inventory;
    
    inventory.addItem("food", 3);
    inventory.addItem("linemate", 2);
    inventory.addItem("deraumere", 1);
    
    cr_assert_eq(inventory.getItemQuantity("food"), 3);
    cr_assert_eq(inventory.getItemQuantity("linemate"), 2);
    cr_assert_eq(inventory.getItemQuantity("deraumere"), 1);
}

Test(Inventory, add_same_item_multiple_times) {
    Inventory inventory;
    
    inventory.addItem("food", 3);
    inventory.addItem("food", 2);
    inventory.addItem("food", 1);
    
    cr_assert_eq(inventory.getItemQuantity("food"), 6);
}

Test(Inventory, add_new_item_type) {
    Inventory inventory;
    
    inventory.addItem("custom_item", 10);
    cr_assert_eq(inventory.getItemQuantity("custom_item"), 10);
}

Test(Inventory, remove_item_exact_amount) {
    Inventory inventory;
    
    inventory.addItem("food", 5);
    inventory.removeItem("food", 5);
    
    cr_assert_eq(inventory.getItemQuantity("food"), 0);
}

Test(Inventory, remove_item_partial_amount) {
    Inventory inventory;
    
    inventory.addItem("food", 10);
    inventory.removeItem("food", 3);
    
    cr_assert_eq(inventory.getItemQuantity("food"), 7);
}

Test(Inventory, remove_item_more_than_available) {
    Inventory inventory;
    
    inventory.addItem("food", 5);
    inventory.removeItem("food", 10);  // Try to remove more than available
    
    cr_assert_eq(inventory.getItemQuantity("food"), 5);  // Should remain unchanged
}

Test(Inventory, remove_nonexistent_item) {
    Inventory inventory;
    
    inventory.removeItem("nonexistent", 5);  // Should not crash
    cr_assert_eq(inventory.getItemQuantity("nonexistent"), 0);
}

Test(Inventory, get_quantity_nonexistent_item) {
    Inventory inventory;
    
    cr_assert_eq(inventory.getItemQuantity("nonexistent"), 0);
}

Test(Inventory, zero_quantity_operations) {
    Inventory inventory;
    
    inventory.addItem("food", 0);
    cr_assert_eq(inventory.getItemQuantity("food"), 0);
    
    inventory.removeItem("food", 0);
    cr_assert_eq(inventory.getItemQuantity("food"), 0);
}

Test(Inventory, large_quantities) {
    Inventory inventory;
    
    size_t large_quantity = 1000000;
    inventory.addItem("food", large_quantity);
    cr_assert_eq(inventory.getItemQuantity("food"), large_quantity);
    
    inventory.removeItem("food", large_quantity / 2);
    cr_assert_eq(inventory.getItemQuantity("food"), large_quantity / 2);
}

Test(Inventory, all_zappy_items) {
    Inventory inventory;
    
    // Test all official Zappy items
    const std::vector<std::string> zappy_items = {
        "food", "linemate", "deraumere", "sibur", 
        "mendiane", "phiras", "thystame", "egg"
    };
    
    for (size_t i = 0; i < zappy_items.size(); ++i) {
        inventory.addItem(zappy_items[i], i + 1);
        cr_assert_eq(inventory.getItemQuantity(zappy_items[i]), i + 1);
    }
}

Test(Inventory, mixed_operations_sequence) {
    Inventory inventory;
    
    // Complex sequence of operations
    inventory.addItem("food", 10);
    inventory.addItem("linemate", 5);
    inventory.removeItem("food", 3);
    inventory.addItem("food", 2);
    inventory.removeItem("linemate", 1);
    
    cr_assert_eq(inventory.getItemQuantity("food"), 9);
    cr_assert_eq(inventory.getItemQuantity("linemate"), 4);
}

Test(Inventory, empty_string_item) {
    Inventory inventory;
    
    inventory.addItem("", 5);
    cr_assert_eq(inventory.getItemQuantity(""), 5);
    
    inventory.removeItem("", 2);
    cr_assert_eq(inventory.getItemQuantity(""), 3);
}

Test(Inventory, special_character_items) {
    Inventory inventory;
    
    inventory.addItem("item_with_underscore", 3);
    inventory.addItem("item-with-dash", 2);
    inventory.addItem("item with space", 1);
    
    cr_assert_eq(inventory.getItemQuantity("item_with_underscore"), 3);
    cr_assert_eq(inventory.getItemQuantity("item-with-dash"), 2);
    cr_assert_eq(inventory.getItemQuantity("item with space"), 1);
}
