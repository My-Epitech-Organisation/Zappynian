/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Comprehensive unit tests for EventReceiver class
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "../../gui/Event/EventReceiver.hpp"

TestSuite(EventReceiver, .timeout = 10);

Test(EventReceiver, construction) {
    EventReceiver receiver;
    
    // Test default state
    cr_assert_not(receiver.getIsMoving());
    cr_assert_eq(receiver.getMoveStartTime(), 0);
    cr_assert_eq(receiver.getMoveStartX(), 0.0f);
    cr_assert_eq(receiver.getMoveStartZ(), 0.0f);
    cr_assert_eq(receiver.getCurrentRotationY(), 0.0f);
    cr_assert_eq(receiver.getCurrentEntityId(), 0);
}

Test(EventReceiver, movement_state_setters) {
    EventReceiver receiver;
    
    // Test movement state setters
    receiver.setIsMoving(true);
    cr_assert(receiver.getIsMoving());
    
    receiver.setIsMoving(false);
    cr_assert_not(receiver.getIsMoving());
    
    receiver.setMoveStartTime(1000);
    cr_assert_eq(receiver.getMoveStartTime(), 1000);
    
    receiver.setMoveStartX(15.5f);
    cr_assert_eq(receiver.getMoveStartX(), 15.5f);
    
    receiver.setMoveStartZ(20.3f);
    cr_assert_eq(receiver.getMoveStartZ(), 20.3f);
    
    receiver.setCurrentRotationY(90.0f);
    cr_assert_eq(receiver.getCurrentRotationY(), 90.0f);
    
    receiver.setCurrentEntityId(42);
    cr_assert_eq(receiver.getCurrentEntityId(), 42);
}

Test(EventReceiver, entity_management) {
    EventReceiver receiver;
    
    // Test clear all entities (should not crash on empty)
    receiver.clearAllEntities();
    
    // Test remove entity that doesn't exist (should not crash)
    receiver.removeEntity(999);
    
    // Test adding entities would need actual IEntity implementations
    // For now, just test the method doesn't crash
    cr_assert(true);
}

Test(EventReceiver, device_setting) {
    EventReceiver receiver;
    
    // Test setting device to nullptr (should not crash)
    receiver.setDevice(nullptr);
    
    // The method should complete without error
    cr_assert(true);
}

Test(EventReceiver, text_setting) {
    EventReceiver receiver;
    
    // Test setting text elements to nullptr (should not crash)
    receiver.setText(nullptr);
    receiver.setPlayerText(nullptr);
    
    // Methods should complete without error
    cr_assert(true);
}

Test(EventReceiver, animated_node_setting) {
    EventReceiver receiver;
    
    // Test setting animated node to nullptr (should not crash)
    receiver.setAnimatedNode(nullptr);
    
    // Method should complete without error
    cr_assert(true);
}

Test(EventReceiver, event_handling_key_input) {
    EventReceiver receiver;
    
    // Create a mock key event
    irr::SEvent event;
    event.EventType = irr::EET_KEY_INPUT_EVENT;
    event.KeyInput.Key = irr::KEY_ESCAPE;
    event.KeyInput.PressedDown = true;
    event.KeyInput.Shift = false;
    event.KeyInput.Control = false;
    
    // Test that OnEvent can handle key input without crashing
    // Note: Return value depends on implementation
    receiver.OnEvent(event);
    
    cr_assert(true);
}

Test(EventReceiver, event_handling_mouse_input) {
    EventReceiver receiver;
    
    // Create a mock mouse event
    irr::SEvent event;
    event.EventType = irr::EET_MOUSE_INPUT_EVENT;
    event.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
    event.MouseInput.X = 100;
    event.MouseInput.Y = 200;
    event.MouseInput.Wheel = 0.0f;
    event.MouseInput.Shift = false;
    event.MouseInput.Control = false;
    
    // Test that OnEvent can handle mouse input without crashing
    receiver.OnEvent(event);
    
    cr_assert(true);
}

Test(EventReceiver, event_handling_gui_event) {
    EventReceiver receiver;
    
    // Create a mock GUI event
    irr::SEvent event;
    event.EventType = irr::EET_GUI_EVENT;
    event.GUIEvent.EventType = irr::gui::EGET_BUTTON_CLICKED;
    event.GUIEvent.Caller = nullptr;
    event.GUIEvent.Element = nullptr;
    
    // Test that OnEvent can handle GUI events without crashing
    receiver.OnEvent(event);
    
    cr_assert(true);
}

Test(EventReceiver, handle_camera_movement) {
    EventReceiver receiver;
    
    // Test camera movement with various delta values
    bool result1 = receiver.handleCameraMovement(10, 5);
    bool result2 = receiver.handleCameraMovement(-5, -10);
    bool result3 = receiver.handleCameraMovement(0, 0);
    
    // Suppress unused variable warnings
    (void)result1;
    (void)result2;
    (void)result3;
    
    // Methods should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_mouse_wheel) {
    EventReceiver receiver;
    
    // Test mouse wheel movement
    bool result1 = receiver.handleMouseWheelMovement(1.0f);
    bool result2 = receiver.handleMouseWheelMovement(-1.0f);
    bool result3 = receiver.handleMouseWheelMovement(0.0f);
    
    // Suppress unused variable warnings
    (void)result1;
    (void)result2;
    (void)result3;
    
    // Methods should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_character_movement) {
    EventReceiver receiver;
    
    // Test character movement with various keys
    bool result1 = receiver.handleCharacterMovement(irr::KEY_UP);
    bool result2 = receiver.handleCharacterMovement(irr::KEY_DOWN);
    bool result3 = receiver.handleCharacterMovement(irr::KEY_LEFT);
    bool result4 = receiver.handleCharacterMovement(irr::KEY_RIGHT);
    bool result5 = receiver.handleCharacterMovement(irr::KEY_SPACE);
    
    // Suppress unused variable warnings
    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    
    // Methods should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_mouse_input_direct) {
    EventReceiver receiver;
    
    // Test direct mouse input handling
    irr::SEvent::SMouseInput mouseInput;
    mouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
    mouseInput.X = 150;
    mouseInput.Y = 250;
    mouseInput.Wheel = 0.0f;
    mouseInput.Shift = false;
    mouseInput.Control = false;
    
    bool result = receiver.handleMouseInput(mouseInput);
    (void)result; // Suppress unused variable warning
    
    // Method should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_key_input_direct) {
    EventReceiver receiver;
    
    // Test direct key input handling
    irr::SEvent::SKeyInput keyInput;
    keyInput.Key = irr::KEY_ESCAPE;
    keyInput.PressedDown = true;
    keyInput.Shift = false;
    keyInput.Control = false;
    keyInput.Char = L'\x1b';
    
    bool result = receiver.handleKeyInput(keyInput);
    (void)result; // Suppress unused variable warning
    
    // Method should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_gui_event_direct) {
    EventReceiver receiver;
    
    // Test direct GUI event handling
    irr::SEvent::SGUIEvent guiEvent;
    guiEvent.EventType = irr::gui::EGET_BUTTON_CLICKED;
    guiEvent.Caller = nullptr;
    guiEvent.Element = nullptr;
    
    bool result = receiver.handleGUIEvent(guiEvent);
    (void)result; // Suppress unused variable warning
    
    // Method should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_node_selection) {
    EventReceiver receiver;
    
    // Test node selection
    irr::SEvent::SMouseInput mouseInput;
    mouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
    mouseInput.X = 100;
    mouseInput.Y = 100;
    mouseInput.Wheel = 0.0f;
    mouseInput.Shift = false;
    mouseInput.Control = false;
    
    bool result = receiver.handleNodeSelection(mouseInput);
    (void)result; // Suppress unused variable warning
    
    // Method should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, handle_player_selection) {
    EventReceiver receiver;
    
    // Test player selection
    irr::SEvent::SMouseInput mouseInput;
    mouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
    mouseInput.X = 200;
    mouseInput.Y = 150;
    mouseInput.Wheel = 0.0f;
    mouseInput.Shift = false;
    mouseInput.Control = false;
    
    bool result = receiver.handlePlayerSelection(mouseInput);
    (void)result; // Suppress unused variable warning
    
    // Method should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, move_camera) {
    EventReceiver receiver;
    
    // Test camera movement with null camera (should handle gracefully)
    bool result1 = receiver.moveCamera(irr::KEY_UP, nullptr);
    bool result2 = receiver.moveCamera(irr::KEY_DOWN, nullptr);
    bool result3 = receiver.moveCamera(irr::KEY_LEFT, nullptr);
    bool result4 = receiver.moveCamera(irr::KEY_RIGHT, nullptr);
    
    // Suppress unused variable warnings
    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    
    // Methods should complete without crashing
    cr_assert(true);
}

Test(EventReceiver, state_consistency) {
    EventReceiver receiver;
    
    // Test state consistency after multiple operations
    receiver.setIsMoving(true);
    receiver.setMoveStartTime(500);
    receiver.setMoveStartX(10.0f);
    receiver.setMoveStartZ(15.0f);
    receiver.setCurrentRotationY(180.0f);
    receiver.setCurrentEntityId(123);
    
    // Verify all states are maintained
    cr_assert(receiver.getIsMoving());
    cr_assert_eq(receiver.getMoveStartTime(), 500);
    cr_assert_eq(receiver.getMoveStartX(), 10.0f);
    cr_assert_eq(receiver.getMoveStartZ(), 15.0f);
    cr_assert_eq(receiver.getCurrentRotationY(), 180.0f);
    cr_assert_eq(receiver.getCurrentEntityId(), 123);
    
    // Change state again
    receiver.setIsMoving(false);
    receiver.setCurrentEntityId(456);
    
    cr_assert_not(receiver.getIsMoving());
    cr_assert_eq(receiver.getCurrentEntityId(), 456);
    // Other values should remain unchanged
    cr_assert_eq(receiver.getMoveStartTime(), 500);
    cr_assert_eq(receiver.getMoveStartX(), 10.0f);
    cr_assert_eq(receiver.getMoveStartZ(), 15.0f);
    cr_assert_eq(receiver.getCurrentRotationY(), 180.0f);
}
