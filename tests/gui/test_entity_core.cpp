/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Entity components without Irrlicht dependencies
*/

#include <criterion/criterion.h>
#include "irrlicht_mocks.hpp"  // Use mocks to avoid Irrlicht dependencies
#include "Entities/AEntity.hpp"
#include "Entities/Inventory.hpp"

// Simple mock entity for testing AEntity functionality
class SimpleTestEntity : public AEntity {
public:
    SimpleTestEntity() : AEntity() {}
    
    void createNode(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver) override {
        // Mock implementation that doesn't require real Irrlicht device
        (void)smgr;
        (void)driver;
    }
};

TestSuite(entity_core);

Test(entity_core, aentity_id_management) {
    SimpleTestEntity entity;
    
    // Test default ID
    cr_assert_eq(entity.getId(), 0, "Default ID should be 0");
    
    // Test setting ID
    entity.setId(42);
    cr_assert_eq(entity.getId(), 42, "ID should be set correctly");
    
    // Test setting negative ID
    entity.setId(-1);
    cr_assert_eq(entity.getId(), -1, "Negative ID should be accepted");
}

Test(entity_core, aentity_name_management) {
    SimpleTestEntity entity;
    
    // Test default name
    cr_assert_str_eq(entity.getName().c_str(), "unknown", "Default name should be 'unknown'");
    
    // Test setting name
    entity.setName("TestEntity");
    cr_assert_str_eq(entity.getName().c_str(), "TestEntity", "Name should be set correctly");
    
    // Test empty name
    entity.setName("");
    cr_assert_str_eq(entity.getName().c_str(), "", "Empty name should be accepted");
    
    // Test name with spaces
    entity.setName("Test Entity");
    cr_assert_str_eq(entity.getName().c_str(), "Test Entity", "Name with spaces should work");
}

Test(entity_core, aentity_level_management) {
    SimpleTestEntity entity;
    
    // Test default level
    cr_assert_eq(entity.getLevel(), 1, "Default level should be 1");
    
    // Test setting level
    entity.setLevel(5);
    cr_assert_eq(entity.getLevel(), 5, "Level should be set correctly");
    
    // Test maximum level (Zappy goes up to level 8)
    entity.setLevel(8);
    cr_assert_eq(entity.getLevel(), 8, "Maximum level should be accepted");
    
    // Test level 0
    entity.setLevel(0);
    cr_assert_eq(entity.getLevel(), 0, "Level 0 should be accepted");
}

Test(entity_core, aentity_position_management) {
    SimpleTestEntity entity;
    
    // Test default position
    irr::core::vector3df defaultPos = entity.getPosition();
    cr_assert_float_eq(defaultPos.X, 0.0f, 0.001f, "Default X position should be 0");
    cr_assert_float_eq(defaultPos.Y, 0.0f, 0.001f, "Default Y position should be 0");
    cr_assert_float_eq(defaultPos.Z, 0.0f, 0.001f, "Default Z position should be 0");
    
    // Test setting position
    irr::core::vector3df newPos(10.5f, 20.3f, -5.7f);
    entity.setPosition(newPos);
    irr::core::vector3df retrievedPos = entity.getPosition();
    cr_assert_float_eq(retrievedPos.X, 10.5f, 0.001f, "X position should be set correctly");
    cr_assert_float_eq(retrievedPos.Y, 20.3f, 0.001f, "Y position should be set correctly");
    cr_assert_float_eq(retrievedPos.Z, -5.7f, 0.001f, "Z position should be set correctly");
}

Test(entity_core, aentity_scale_management) {
    SimpleTestEntity entity;
    
    // Test default scale
    irr::core::vector3df defaultScale = entity.getScale();
    cr_assert_float_eq(defaultScale.X, 1.0f, 0.001f, "Default X scale should be 1");
    cr_assert_float_eq(defaultScale.Y, 1.0f, 0.001f, "Default Y scale should be 1");
    cr_assert_float_eq(defaultScale.Z, 1.0f, 0.001f, "Default Z scale should be 1");
    
    // Test setting scale
    irr::core::vector3df newScale(2.0f, 0.5f, 3.0f);
    entity.setScale(newScale);
    irr::core::vector3df retrievedScale = entity.getScale();
    cr_assert_float_eq(retrievedScale.X, 2.0f, 0.001f, "X scale should be set correctly");
    cr_assert_float_eq(retrievedScale.Y, 0.5f, 0.001f, "Y scale should be set correctly");
    cr_assert_float_eq(retrievedScale.Z, 3.0f, 0.001f, "Z scale should be set correctly");
}

Test(entity_core, aentity_direction_management) {
    SimpleTestEntity entity;
    
    // Test default direction
    cr_assert_eq(entity.getDirection(), Direction::NORTH, "Default direction should be NORTH");
    
    // Test setting each direction
    entity.setDirection(Direction::EAST);
    cr_assert_eq(entity.getDirection(), Direction::EAST, "Direction should be EAST");
    
    entity.setDirection(Direction::SOUTH);
    cr_assert_eq(entity.getDirection(), Direction::SOUTH, "Direction should be SOUTH");
    
    entity.setDirection(Direction::WEST);
    cr_assert_eq(entity.getDirection(), Direction::WEST, "Direction should be WEST");
    
    entity.setDirection(Direction::NORTH);
    cr_assert_eq(entity.getDirection(), Direction::NORTH, "Direction should be NORTH again");
}

Test(entity_core, aentity_inventory_access) {
    SimpleTestEntity entity;
    
    // Test getting inventory reference
    Inventory& inv = entity.getInventory();
    
    // Test that inventory starts empty
    cr_assert_eq(inv.getItemQuantity("food"), 0, "Initial inventory should be empty");
    
    // Test modifying inventory through entity
    inv.addItem("food", 10);
    cr_assert_eq(inv.getItemQuantity("food"), 10, "Inventory should be modifiable through entity");
    
    // Test const inventory access
    const SimpleTestEntity& constEntity = entity;
    const Inventory& constInv = constEntity.getInventory();
    cr_assert_eq(constInv.getItemQuantity("food"), 10, "Const inventory access should work");
}

Test(entity_core, multiple_entities_independence) {
    SimpleTestEntity entity1, entity2;
    
    // Set different properties
    entity1.setId(1);
    entity1.setName("Entity1");
    entity1.setLevel(3);
    
    entity2.setId(2);
    entity2.setName("Entity2");
    entity2.setLevel(5);
    
    // Verify independence
    cr_assert_eq(entity1.getId(), 1, "Entity1 ID should be independent");
    cr_assert_eq(entity2.getId(), 2, "Entity2 ID should be independent");
    cr_assert_str_eq(entity1.getName().c_str(), "Entity1", "Entity1 name should be independent");
    cr_assert_str_eq(entity2.getName().c_str(), "Entity2", "Entity2 name should be independent");
    cr_assert_eq(entity1.getLevel(), 3, "Entity1 level should be independent");
    cr_assert_eq(entity2.getLevel(), 5, "Entity2 level should be independent");
    
    // Test inventory independence
    entity1.getInventory().addItem("food", 10);
    entity2.getInventory().addItem("food", 20);
    
    cr_assert_eq(entity1.getInventory().getItemQuantity("food"), 10, "Entity1 inventory should be independent");
    cr_assert_eq(entity2.getInventory().getItemQuantity("food"), 20, "Entity2 inventory should be independent");
}
