/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for AEntity class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "irrlicht_mocks.hpp"
#include "Entities/AEntity.hpp"
#include "Entities/Direction.hpp"
#include <memory>

// Test implementation of AEntity for testing
class TestEntity : public AEntity {
public:
    TestEntity() : AEntity() {}
    
    void createNode(irr::scene::ISceneManager *smgr, irr::video::IVideoDriver *driver) override {
        // Mock implementation - don't actually create anything
    }
    
    // Expose protected members for testing
    void setScale(const irr::core::vector3df& scale) { scale_ = scale; }
    irr::core::vector3df getScale() const { return scale_; }
    
    void setLevel(int level) { level_ = level; }
    int getLevel() const { return level_; }
    
    Inventory& getInventory() { return inventory_; }
    const Inventory& getInventory() const { return inventory_; }
};

TestSuite(aentity);

Test(aentity, default_constructor) {
    TestEntity entity;
    
    cr_assert_eq(entity.getId(), 0, "Default ID should be 0");
    cr_assert_str_eq(entity.getName().c_str(), "unknown", "Default name should be 'unknown'");
    cr_assert_eq(entity.getLevel(), 1, "Default level should be 1");
}

Test(aentity, set_get_id) {
    TestEntity entity;
    
    entity.setId(42);
    cr_assert_eq(entity.getId(), 42, "ID should be set correctly");
    
    entity.setId(-5);
    cr_assert_eq(entity.getId(), -5, "Negative ID should be set correctly");
    
    entity.setId(0);
    cr_assert_eq(entity.getId(), 0, "Zero ID should be set correctly");
}

Test(aentity, set_get_name) {
    TestEntity entity;
    
    entity.setName("Player1");
    cr_assert_str_eq(entity.getName().c_str(), "Player1", "Name should be set correctly");
    
    entity.setName("");
    cr_assert_str_eq(entity.getName().c_str(), "", "Empty name should be set correctly");
    
    entity.setName("Very long entity name with special characters @#$%");
    cr_assert_str_eq(entity.getName().c_str(), "Very long entity name with special characters @#$%", 
                     "Complex name should be set correctly");
}

Test(aentity, set_get_position) {
    TestEntity entity;
    
    irr::core::vector3df pos1(1.0f, 2.0f, 3.0f);
    entity.setPosition(pos1);
    irr::core::vector3df retrieved = entity.getPosition();
    cr_assert_float_eq(retrieved.X, 1.0f, 0.001f, "X position should be set correctly");
    cr_assert_float_eq(retrieved.Y, 2.0f, 0.001f, "Y position should be set correctly");
    cr_assert_float_eq(retrieved.Z, 3.0f, 0.001f, "Z position should be set correctly");
    
    irr::core::vector3df pos2(-10.5f, 0.0f, 100.25f);
    entity.setPosition(pos2);
    retrieved = entity.getPosition();
    cr_assert_float_eq(retrieved.X, -10.5f, 0.001f, "Negative X position should be set correctly");
    cr_assert_float_eq(retrieved.Y, 0.0f, 0.001f, "Zero Y position should be set correctly");
    cr_assert_float_eq(retrieved.Z, 100.25f, 0.001f, "Large Z position should be set correctly");
}

Test(aentity, set_get_scale) {
    TestEntity entity;
    
    irr::core::vector3df scale1(2.0f, 1.5f, 0.5f);
    entity.setScale(scale1);
    irr::core::vector3df retrieved = entity.getScale();
    cr_assert_float_eq(retrieved.X, 2.0f, 0.001f, "X scale should be set correctly");
    cr_assert_float_eq(retrieved.Y, 1.5f, 0.001f, "Y scale should be set correctly");
    cr_assert_float_eq(retrieved.Z, 0.5f, 0.001f, "Z scale should be set correctly");
}

Test(aentity, set_get_level) {
    TestEntity entity;
    
    entity.setLevel(5);
    cr_assert_eq(entity.getLevel(), 5, "Level should be set correctly");
    
    entity.setLevel(1);
    cr_assert_eq(entity.getLevel(), 1, "Level 1 should be set correctly");
    
    entity.setLevel(8);
    cr_assert_eq(entity.getLevel(), 8, "Max level should be set correctly");
}

Test(aentity, inventory_access) {
    TestEntity entity;
    
    Inventory& inv = entity.getInventory();
    inv.addItem("food", 5);
    
    const Inventory& const_inv = entity.getInventory();
    cr_assert_eq(const_inv.getItemQuantity("food"), 5, "Inventory should be accessible and modifiable");
}

Test(aentity, create_node_mock) {
    TestEntity entity;
    
    // Test that createNode can be called without crashing
    entity.createNode(nullptr, nullptr);
    cr_assert(true, "createNode mock implementation should not crash");
}

Test(aentity, multiple_entities_independence) {
    TestEntity entity1, entity2;
    
    entity1.setId(1);
    entity1.setName("Entity1");
    entity1.setLevel(3);
    
    entity2.setId(2);
    entity2.setName("Entity2");
    entity2.setLevel(5);
    
    cr_assert_eq(entity1.getId(), 1, "Entity1 ID should remain independent");
    cr_assert_eq(entity2.getId(), 2, "Entity2 ID should remain independent");
    cr_assert_str_eq(entity1.getName().c_str(), "Entity1", "Entity1 name should remain independent");
    cr_assert_str_eq(entity2.getName().c_str(), "Entity2", "Entity2 name should remain independent");
    cr_assert_eq(entity1.getLevel(), 3, "Entity1 level should remain independent");
    cr_assert_eq(entity2.getLevel(), 5, "Entity2 level should remain independent");
}

Test(aentity, position_edge_cases) {
    TestEntity entity;
    
    // Test extreme values
    irr::core::vector3df extreme_pos(999999.0f, -999999.0f, 0.000001f);
    entity.setPosition(extreme_pos);
    irr::core::vector3df retrieved = entity.getPosition();
    cr_assert_float_eq(retrieved.X, 999999.0f, 0.1f, "Extreme positive X should work");
    cr_assert_float_eq(retrieved.Y, -999999.0f, 0.1f, "Extreme negative Y should work");
    cr_assert_float_eq(retrieved.Z, 0.000001f, 0.000001f, "Very small Z should work");
}
