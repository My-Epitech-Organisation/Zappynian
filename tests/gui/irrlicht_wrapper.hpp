/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Header wrapper to force mock usage in tests
*/

#pragma once

#ifdef MOCK_IRRLICHT
// Force our mock to be used instead of real Irrlicht
#include "irrlicht_mocks.hpp"

// Alias for common Irrlicht includes
#define irrlicht_h irrlicht_mocks_hpp
#define IRRLICHT_H irrlicht_mocks_hpp

#else
// Normal compilation - use real Irrlicht
#include <irrlicht/irrlicht.h>
#endif
