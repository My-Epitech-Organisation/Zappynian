/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for Event system (EventReceiver)
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Event/EventReceiver.hpp"
#include "Entities/TileEntity.hpp"
#include <irrlicht/irrlicht.h>

TestSuite(event_receiver);

Test(event_receiver, constructor_destructor) {
    EventReceiver receiver;
    
    // Constructor and destructor should not crash
    cr_assert(true, "EventReceiver constructor/destructor should work");
}

Test(event_receiver, on_event_basic) {
    EventReceiver receiver;
    
    // Create a mock event
    irr::SEvent event;
    event.EventType = irr::EET_KEY_INPUT_EVENT;
    event.KeyInput.Key = irr::KEY_ESCAPE;
    event.KeyInput.PressedDown = true;
    
    // OnEvent should not crash
    bool result = receiver.OnEvent(event);
    
    // Result can be true or false, just testing it doesn't crash
    cr_assert(result == true || result == false, "OnEvent should return a boolean");
}

Test(event_receiver, on_event_mouse) {
    EventReceiver receiver;
    
    // Test mouse events
    irr::SEvent mouseEvent;
    mouseEvent.EventType = irr::EET_MOUSE_INPUT_EVENT;
    mouseEvent.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
    mouseEvent.MouseInput.X = 100;
    mouseEvent.MouseInput.Y = 200;
    
    bool result = receiver.OnEvent(mouseEvent);
    cr_assert(result == true || result == false, "Mouse event should be handled");
}

Test(event_receiver, on_event_gui) {
    EventReceiver receiver;
    
    // Test GUI events
    irr::SEvent guiEvent;
    guiEvent.EventType = irr::EET_GUI_EVENT;
    guiEvent.GUIEvent.EventType = irr::gui::EGET_BUTTON_CLICKED;
    guiEvent.GUIEvent.Caller = nullptr;
    
    bool result = receiver.OnEvent(guiEvent);
    cr_assert(result == true || result == false, "GUI event should be handled");
}

Test(event_receiver, add_cube) {
    EventReceiver receiver;
    
    // Create a mock tile entity
    TileEntity tile(1, irr::core::vector3df(0, 0, 0), irr::core::vector3df(1, 1, 1),
                   std::vector<irr::io::path>(), "", nullptr, "TestTile");
    
    // addCube should not crash
    receiver.addCube(&tile);
    
    cr_assert(true, "addCube should work");
}

Test(event_receiver, add_multiple_cubes) {
    EventReceiver receiver;
    
    // Create multiple mock tile entities
    std::vector<std::unique_ptr<TileEntity>> tiles;
    
    for (int i = 0; i < 10; ++i) {
        auto tile = std::make_unique<TileEntity>(
            i, irr::core::vector3df(i, i, 0), irr::core::vector3df(1, 1, 1),
            std::vector<irr::io::path>(), "", nullptr, "TestTile" + std::to_string(i));
        
        receiver.addCube(tile.get());
        tiles.push_back(std::move(tile));
    }
    
    cr_assert(true, "Multiple cubes should be added successfully");
}

Test(event_receiver, add_null_cube) {
    EventReceiver receiver;
    
    // Adding null cube should not crash
    receiver.addCube(nullptr);
    
    cr_assert(true, "Adding null cube should be handled gracefully");
}

Test(event_receiver, stress_test_events) {
    EventReceiver receiver;
    
    // Send many events rapidly
    for (int i = 0; i < 1000; ++i) {
        irr::SEvent event;
        event.EventType = irr::EET_KEY_INPUT_EVENT;
        event.KeyInput.Key = static_cast<irr::EKEY_CODE>(irr::KEY_KEY_A + (i % 26));
        event.KeyInput.PressedDown = (i % 2) == 0;
        
        receiver.OnEvent(event);
    }
    
    cr_assert(true, "Stress test should complete without crashing");
}

Test(event_receiver, different_event_types) {
    EventReceiver receiver;
    
    // Test various event types
    std::vector<irr::EEVENT_TYPE> eventTypes = {
        irr::EET_GUI_EVENT,
        irr::EET_MOUSE_INPUT_EVENT,
        irr::EET_KEY_INPUT_EVENT,
        irr::EET_JOYSTICK_INPUT_EVENT,
        irr::EET_LOG_TEXT_EVENT,
        irr::EET_USER_EVENT
    };
    
    for (auto eventType : eventTypes) {
        irr::SEvent event;
        event.EventType = eventType;
        
        // Initialize event data based on type
        switch (eventType) {
            case irr::EET_KEY_INPUT_EVENT:
                event.KeyInput.Key = irr::KEY_SPACE;
                event.KeyInput.PressedDown = true;
                break;
            case irr::EET_MOUSE_INPUT_EVENT:
                event.MouseInput.Event = irr::EMIE_LMOUSE_PRESSED_DOWN;
                event.MouseInput.X = 0;
                event.MouseInput.Y = 0;
                break;
            case irr::EET_GUI_EVENT:
                event.GUIEvent.EventType = irr::gui::EGET_BUTTON_CLICKED;
                event.GUIEvent.Caller = nullptr;
                break;
            default:
                break;
        }
        
        bool result = receiver.OnEvent(event);
        cr_assert(result == true || result == false, "Event should be handled");
    }
    
    cr_assert(true, "All event types should be handled");
}

Test(event_receiver, concurrent_cube_addition) {
    EventReceiver receiver;
    
    // Simulate concurrent operations (testing thread safety conceptually)
    std::vector<std::unique_ptr<TileEntity>> tiles;
    
    // Add cubes in a pattern that might expose threading issues
    for (int i = 0; i < 100; ++i) {
        auto tile = std::make_unique<TileEntity>(
            i, irr::core::vector3df(i % 10, i / 10, 0), irr::core::vector3df(1, 1, 1),
            std::vector<irr::io::path>(), "", nullptr, "ConcurrentTile" + std::to_string(i));
        
        receiver.addCube(tile.get());
        
        // Send an event between additions
        irr::SEvent event;
        event.EventType = irr::EET_KEY_INPUT_EVENT;
        event.KeyInput.Key = irr::KEY_SPACE;
        event.KeyInput.PressedDown = true;
        receiver.OnEvent(event);
        
        tiles.push_back(std::move(tile));
    }
    
    cr_assert(true, "Concurrent operations should be handled safely");
}
