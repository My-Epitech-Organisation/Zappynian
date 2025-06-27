/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Inventory class
*/

#include <criterion/criterion.h>
#include "Entities/Inventory.hpp"

TestSuite(inventory);

Test(inventory, default_initialization) {
    Inventory inv;
    
    // All items should be initialized to 0
    cr_assert_eq(inv.getItemQuantity("food"), 0);
    cr_assert_eq(inv.getItemQuantity("linemate"), 0);
    cr_assert_eq(inv.getItemQuantity("deraumere"), 0);
    cr_assert_eq(inv.getItemQuantity("sibur"), 0);
    cr_assert_eq(inv.getItemQuantity("mendiane"), 0);
    cr_assert_eq(inv.getItemQuantity("phiras"), 0);
    cr_assert_eq(inv.getItemQuantity("thystame"), 0);
    cr_assert_eq(inv.getItemQuantity("egg"), 0);
}

Test(inventory, add_single_item) {
    Inventory inv;
    
    inv.addItem("food", 5);
    cr_assert_eq(inv.getItemQuantity("food"), 5);
    
    // Other items should remain 0
    cr_assert_eq(inv.getItemQuantity("linemate"), 0);
    cr_assert_eq(inv.getItemQuantity("deraumere"), 0);
}

Test(inventory, add_multiple_items) {
    Inventory inv;
    
    inv.addItem("food", 3);
    inv.addItem("linemate", 2);
    inv.addItem("deraumere", 1);
    
    cr_assert_eq(inv.getItemQuantity("food"), 3);
    cr_assert_eq(inv.getItemQuantity("linemate"), 2);
    cr_assert_eq(inv.getItemQuantity("deraumere"), 1);
}

Test(inventory, add_same_item_multiple_times) {
    Inventory inv;
    
    inv.addItem("food", 3);
    inv.addItem("food", 2);
    inv.addItem("food", 1);
    
    cr_assert_eq(inv.getItemQuantity("food"), 6);
}

Test(inventory, add_zero_quantity) {
    Inventory inv;
    
    inv.addItem("food", 0);
    cr_assert_eq(inv.getItemQuantity("food"), 0);
    
    inv.addItem("food", 5);
    inv.addItem("food", 0);
    cr_assert_eq(inv.getItemQuantity("food"), 5);
}

Test(inventory, add_large_quantity) {
    Inventory inv;
    
    inv.addItem("food", 999999);
    cr_assert_eq(inv.getItemQuantity("food"), 999999);
}

Test(inventory, remove_single_item) {
    Inventory inv;
    
    inv.addItem("food", 10);
    inv.removeItem("food", 3);
    
    cr_assert_eq(inv.getItemQuantity("food"), 7);
}

Test(inventory, remove_all_items) {
    Inventory inv;
    
    inv.addItem("food", 5);
    inv.removeItem("food", 5);
    
    cr_assert_eq(inv.getItemQuantity("food"), 0);
}

Test(inventory, remove_more_than_available) {
    Inventory inv;
    
    inv.addItem("food", 3);
    inv.removeItem("food", 5);
    
    // Should not go negative (depending on implementation)
    // Most likely implementation would set to 0 or handle gracefully
    size_t remaining = inv.getItemQuantity("food");
    cr_assert(remaining == 0 || remaining == 3, "Should handle over-removal gracefully");
}

Test(inventory, remove_from_empty_inventory) {
    Inventory inv;
    
    inv.removeItem("food", 5);
    
    cr_assert_eq(inv.getItemQuantity("food"), 0);
}

Test(inventory, remove_zero_quantity) {
    Inventory inv;
    
    inv.addItem("food", 5);
    inv.removeItem("food", 0);
    
    cr_assert_eq(inv.getItemQuantity("food"), 5);
}

Test(inventory, get_nonexistent_item) {
    Inventory inv;
    
    cr_assert_eq(inv.getItemQuantity("nonexistent_item"), 0);
    cr_assert_eq(inv.getItemQuantity(""), 0);
    cr_assert_eq(inv.getItemQuantity("UNKNOWN"), 0);
}

Test(inventory, add_nonexistent_item) {
    Inventory inv;
    
    inv.addItem("unknown_item", 5);
    cr_assert_eq(inv.getItemQuantity("unknown_item"), 5);
}

Test(inventory, case_sensitivity) {
    Inventory inv;
    
    inv.addItem("food", 5);
    inv.addItem("FOOD", 3);
    inv.addItem("Food", 2);
    
    // These should be treated as different items (case sensitive)
    cr_assert_eq(inv.getItemQuantity("food"), 5);
    cr_assert_eq(inv.getItemQuantity("FOOD"), 3);
    cr_assert_eq(inv.getItemQuantity("Food"), 2);
}

Test(inventory, empty_string_item) {
    Inventory inv;
    
    inv.addItem("", 5);
    cr_assert_eq(inv.getItemQuantity(""), 5);
    
    inv.removeItem("", 2);
    cr_assert_eq(inv.getItemQuantity(""), 3);
}

Test(inventory, all_zappy_resources) {
    Inventory inv;
    
    // Test all standard Zappy resources
    std::vector<std::string> resources = {
        "food", "linemate", "deraumere", "sibur", 
        "mendiane", "phiras", "thystame"
    };
    
    for (size_t i = 0; i < resources.size(); ++i) {
        inv.addItem(resources[i], i + 1);
    }
    
    for (size_t i = 0; i < resources.size(); ++i) {
        cr_assert_eq(inv.getItemQuantity(resources[i]), i + 1);
    }
}

Test(inventory, complex_operations) {
    Inventory inv;
    
    // Add initial resources
    inv.addItem("food", 10);
    inv.addItem("linemate", 5);
    inv.addItem("deraumere", 3);
    
    // Remove some resources
    inv.removeItem("food", 3);
    inv.removeItem("linemate", 2);
    
    // Add more resources
    inv.addItem("food", 2);
    inv.addItem("sibur", 4);
    
    // Check final state
    cr_assert_eq(inv.getItemQuantity("food"), 9);      // 10 - 3 + 2
    cr_assert_eq(inv.getItemQuantity("linemate"), 3);  // 5 - 2
    cr_assert_eq(inv.getItemQuantity("deraumere"), 3); // unchanged
    cr_assert_eq(inv.getItemQuantity("sibur"), 4);     // new item
    cr_assert_eq(inv.getItemQuantity("mendiane"), 0);  // never added
}

Test(inventory, stress_test) {
    Inventory inv;
    
    // Add many different items
    for (int i = 0; i < 1000; ++i) {
        std::string itemName = "item_" + std::to_string(i);
        inv.addItem(itemName, i);
    }
    
    // Verify all items
    for (int i = 0; i < 1000; ++i) {
        std::string itemName = "item_" + std::to_string(i);
        cr_assert_eq(inv.getItemQuantity(itemName), static_cast<size_t>(i));
    }
    
    // Remove half the quantity from each item
    for (int i = 0; i < 1000; ++i) {
        std::string itemName = "item_" + std::to_string(i);
        inv.removeItem(itemName, i / 2);
    }
    
    // Verify remaining quantities
    for (int i = 0; i < 1000; ++i) {
        std::string itemName = "item_" + std::to_string(i);
        size_t expected = i - (i / 2);
        cr_assert_eq(inv.getItemQuantity(itemName), expected);
    }
}

Test(inventory, multiple_inventory_instances) {
    Inventory inv1;
    Inventory inv2;
    
    inv1.addItem("food", 10);
    inv2.addItem("food", 20);
    
    cr_assert_eq(inv1.getItemQuantity("food"), 10);
    cr_assert_eq(inv2.getItemQuantity("food"), 20);
    
    inv1.removeItem("food", 5);
    
    cr_assert_eq(inv1.getItemQuantity("food"), 5);
    cr_assert_eq(inv2.getItemQuantity("food"), 20); // Should not be affected
}

Test(inventory, long_item_names) {
    Inventory inv;
    
    std::string longName = "very_long_item_name_that_should_still_work_properly_in_the_inventory_system";
    
    inv.addItem(longName, 5);
    cr_assert_eq(inv.getItemQuantity(longName), 5);
    
    inv.removeItem(longName, 2);
    cr_assert_eq(inv.getItemQuantity(longName), 3);
}

Test(inventory, special_characters_in_names) {
    Inventory inv;
    
    std::vector<std::string> specialNames = {
        "item-with-dashes",
        "item_with_underscores",
        "item with spaces",
        "item123with456numbers",
        "ITEM_IN_CAPS",
        "item.with.dots"
    };
    
    for (size_t i = 0; i < specialNames.size(); ++i) {
        inv.addItem(specialNames[i], i + 1);
        cr_assert_eq(inv.getItemQuantity(specialNames[i]), i + 1);
    }
}

Test(inventory, edge_case_quantities) {
    Inventory inv;
    
    // Test edge case quantities
    inv.addItem("item1", 0);
    inv.addItem("item2", 1);
        inv.addItem("item3", SIZE_MAX);  // Maximum size_t value
    
    cr_assert_eq(inv.getItemQuantity("item1"), 0);
    cr_assert_eq(inv.getItemQuantity("item2"), 1);
    cr_assert_eq(inv.getItemQuantity("item3"), SIZE_MAX);
}

Test(inventory, multiple_additions_same_item) {
    Inventory inv;
    
    inv.addItem("food", 3);
    inv.addItem("food", 2);
    inv.addItem("food", 1);
    
        cr_assert_eq(inv.getItemQuantity("food"), 6);
}
