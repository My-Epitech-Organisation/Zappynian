#!/bin/bash
##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Functional test runner script
##

set -e

echo "=== Running libzappy_net Functional Tests ==="
echo ""

# Build functional tests
echo "Building functional tests..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "ERROR: Failed to build functional tests"
    exit 1
fi

echo "✓ Functional tests built successfully"
echo ""

# Run functional tests
echo "Running functional tests..."
echo "================================"

./functional_tests_zappy_net

echo ""
echo "=== Functional Tests Complete ==="
