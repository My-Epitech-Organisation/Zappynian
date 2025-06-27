/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Tests for NetworkManager and NetworkManagerImpl classes
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "Network/NetworkManager.hpp"
#include "Network/NetworkManagerImpl.hpp"
#include <memory>
#include <thread>
#include <chrono>

TestSuite(network_manager);

Test(network_manager, constructor_destructor) {
    NetworkManager manager;
    
    // Constructor should not crash and initial state should be disconnected
    cr_assert_eq(manager.getState(), ConnectionState::DISCONNECTED, 
                 "Initial state should be DISCONNECTED");
    cr_assert_eq(manager.isConnected(), false, "Should not be connected initially");
}

Test(network_manager, connect_invalid_host) {
    NetworkManager manager;
    
    bool result = manager.connect("invalid.host.example", 9999);
    
    cr_assert_eq(result, false, "Connection to invalid host should fail");
    cr_assert_eq(manager.getState(), ConnectionState::ERROR, "State should be ERROR after failed connection");
    cr_assert_eq(manager.isConnected(), false, "Should not be connected after failed connection");
}

Test(network_manager, connect_invalid_port) {
    NetworkManager manager;
    
    bool result = manager.connect("localhost", -1);
    
    cr_assert_eq(result, false, "Connection with invalid port should fail");
    cr_assert_eq(manager.getState(), ConnectionState::ERROR, "State should be ERROR after invalid port");
}

Test(network_manager, connect_unreachable_port) {
    NetworkManager manager;
    
    bool result = manager.connect("localhost", 99999);
    
    cr_assert_eq(result, false, "Connection to unreachable port should fail");
    cr_assert_eq(manager.isConnected(), false, "Should not be connected to unreachable port");
}

Test(network_manager, disconnect_when_not_connected) {
    NetworkManager manager;
    
    // Should not crash when disconnecting while not connected
    manager.disconnect();
    
    cr_assert_eq(manager.getState(), ConnectionState::DISCONNECTED, 
                 "State should remain DISCONNECTED");
    cr_assert_eq(manager.isConnected(), false, "Should not be connected after disconnect");
}

Test(network_manager, send_message_when_not_connected) {
    NetworkManager manager;
    
    bool result = manager.sendMessage("test message");
    
    cr_assert_eq(result, false, "Sending message when not connected should fail");
}

Test(network_manager, receive_message_when_not_connected) {
    NetworkManager manager;
    
    std::string message = manager.receiveMessage();
    
    cr_assert_str_eq(message.c_str(), "", "Receiving message when not connected should return empty string");
}

Test(network_manager, has_data_when_not_connected) {
    NetworkManager manager;
    
    bool hasData = manager.hasData();
    
    cr_assert_eq(hasData, false, "Should not have data when not connected");
}

Test(network_manager, get_last_error) {
    NetworkManager manager;
    
    // Try to connect to invalid host to generate an error
    manager.connect("invalid.host.example", 9999);
    
    std::string error = manager.getLastError();
    
    cr_assert_neq(error.length(), 0, "Should have an error message after failed connection");
}

Test(network_manager, game_state_access) {
    NetworkManager manager;
    
    const Zappy::GameState& gameState = manager.getGameState();
    
    cr_assert_eq(gameState.isEmpty(), true, "Game state should be empty initially");
}

Test(network_manager, game_state_synchronization) {
    NetworkManager manager;
    
    bool synchronized = manager.isGameStateSynchronized();
    
    cr_assert_eq(synchronized, false, "Game state should not be synchronized initially");
}

Test(network_manager, update_from_server_when_disconnected) {
    NetworkManager manager;
    
    // Should not crash when updating from server while disconnected
    manager.updateFromServer();
    
    cr_assert_eq(manager.getState(), ConnectionState::DISCONNECTED, 
                 "State should remain DISCONNECTED after update attempt");
}

Test(network_manager, handshake_when_not_connected) {
    NetworkManager manager;
    
    bool result = manager.performHandshake();
    
    cr_assert_eq(result, false, "Handshake should fail when not connected");
}

Test(network_manager, multiple_disconnect_calls) {
    NetworkManager manager;
    
    // Multiple disconnect calls should not crash
    manager.disconnect();
    manager.disconnect();
    manager.disconnect();
    
    cr_assert_eq(manager.getState(), ConnectionState::DISCONNECTED, 
                 "State should be DISCONNECTED after multiple disconnect calls");
}

Test(network_manager, state_transitions) {
    NetworkManager manager;
    
    // Initial state
    cr_assert_eq(manager.getState(), ConnectionState::DISCONNECTED, 
                 "Should start in DISCONNECTED state");
    
    // Try to connect (will fail but should change state)
    manager.connect("invalid.host.example", 9999);
    cr_assert_neq(manager.getState(), ConnectionState::DISCONNECTED, 
                  "State should change after connection attempt");
    
    // Disconnect
    manager.disconnect();
    cr_assert_eq(manager.getState(), ConnectionState::DISCONNECTED, 
                 "Should return to DISCONNECTED state after disconnect");
}

Test(network_manager, empty_host_connection) {
    NetworkManager manager;
    
    bool result = manager.connect("", 8080);
    
    cr_assert_eq(result, false, "Connection with empty host should fail");
    cr_assert_eq(manager.getState(), ConnectionState::ERROR, "State should be ERROR with empty host");
}

Test(network_manager, zero_port_connection) {
    NetworkManager manager;
    
    bool result = manager.connect("localhost", 0);
    
    cr_assert_eq(result, false, "Connection with port 0 should fail");
    cr_assert_eq(manager.getState(), ConnectionState::ERROR, "State should be ERROR with port 0");
}

Test(network_manager, send_empty_message) {
    NetworkManager manager;
    
    // Even when not connected, sending empty message should be handled gracefully
    bool result = manager.sendMessage("");
    
    cr_assert_eq(result, false, "Sending empty message should fail");
}

Test(network_manager, send_large_message) {
    NetworkManager manager;
    
    // Create a large message
    std::string largeMessage(10000, 'A');
    
    bool result = manager.sendMessage(largeMessage);
    
    cr_assert_eq(result, false, "Sending large message when not connected should fail");
}

Test(network_manager, concurrent_operations) {
    NetworkManager manager;
    
    // Test concurrent operations (should be thread-safe)
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&manager]() {
            manager.hasData();
            manager.getState();
            manager.isConnected();
            manager.getLastError();
        });
    }
    
    for (auto& thread : threads) {
        thread.join();
    }
    
    // If we reach here without crashing, the test passes
    cr_assert(true, "Concurrent operations should be safe");
}

// Tests for NetworkManagerImpl (internal implementation)
TestSuite(network_manager_impl);

Test(network_manager_impl, constructor_destructor) {
    NetworkManagerImpl impl;
    
    cr_assert_eq(impl.getState(), ConnectionState::DISCONNECTED, 
                 "NetworkManagerImpl should start in DISCONNECTED state");
    cr_assert_eq(impl.isConnected(), false, "Should not be connected initially");
}

Test(network_manager_impl, library_initialization) {
    NetworkManagerImpl impl;
    
    // Library initialization should be handled internally
    bool result = impl.initializeLibrary();
    
    // Result depends on libzappy_net implementation
    // We mainly test that it doesn't crash
    cr_assert(true, "Library initialization should not crash");
}

Test(network_manager_impl, game_state_access) {
    NetworkManagerImpl impl;
    
    const Zappy::GameState& gameState = impl.getGameState();
    Zappy::GameState& mutableGameState = impl.getGameState();
    
    cr_assert_eq(&gameState, &mutableGameState, "Both const and non-const getGameState should return same reference");
    cr_assert_eq(gameState.isEmpty(), true, "Game state should be empty initially");
}

Test(network_manager_impl, synchronization_status) {
    NetworkManagerImpl impl;
    
    bool synchronized = impl.isGameStateSynchronized();
    
    cr_assert_eq(synchronized, false, "Game state should not be synchronized initially");
}

Test(network_manager_impl, connect_invalid_parameters) {
    NetworkManagerImpl impl;
    
    bool result1 = impl.connect("", 8080);
    cr_assert_eq(result1, false, "Connection with empty host should fail");
    
    bool result2 = impl.connect("localhost", -1);
    cr_assert_eq(result2, false, "Connection with negative port should fail");
    
    bool result3 = impl.connect("localhost", 70000);
    cr_assert_eq(result3, false, "Connection with too large port should fail");
}

Test(network_manager_impl, operations_when_disconnected) {
    NetworkManagerImpl impl;
    
    // All these operations should fail gracefully when disconnected
    cr_assert_eq(impl.sendMessage("test"), false, "Send should fail when disconnected");
    cr_assert_str_eq(impl.receiveMessage().c_str(), "", "Receive should return empty when disconnected");
    cr_assert_eq(impl.hasData(), false, "HasData should return false when disconnected");
    cr_assert_eq(impl.performHandshake(), false, "Handshake should fail when disconnected");
}

Test(network_manager_impl, update_from_server_disconnected) {
    NetworkManagerImpl impl;
    
    // Should not crash when updating while disconnected
    impl.updateFromServer();
    
    cr_assert_eq(impl.getState(), ConnectionState::DISCONNECTED, 
                 "State should remain DISCONNECTED after update");
}

Test(network_manager_impl, error_handling) {
    NetworkManagerImpl impl;
    
    // Generate an error by trying to connect to invalid host
    impl.connect("definitely.not.a.real.host.example", 9999);
    
    std::string error = impl.getLastError();
    cr_assert_neq(error.length(), 0, "Should have error message after failed operation");
    
    ConnectionState state = impl.getState();
    cr_assert_eq(state, ConnectionState::ERROR, "State should be ERROR after failed connection");
}
