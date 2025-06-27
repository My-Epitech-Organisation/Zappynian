/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Irrlicht header redirect for testing
*/

#pragma once

// Redirect to our mocks when in testing mode
#ifdef MOCK_IRRLICHT
#include "../irrlicht_mocks.hpp"
#else
// Include the real Irrlicht in normal compilation
#include <irrlicht/irrlicht.h>
#endif
