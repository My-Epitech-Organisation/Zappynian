#!/bin/bash

##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Test runner script with memory leak detection
##

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Building libzappy_net...${NC}"
make -C ../../libzappy_net

echo -e "${YELLOW}Building tests...${NC}"
make

echo -e "${YELLOW}Running tests with Criterion...${NC}"
./tests_zappy_net --verbose

echo -e "${YELLOW}Running tests with Valgrind...${NC}"
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./tests_zappy_net

if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed with 0 memory leaks!${NC}"
else
    echo -e "${RED}✗ Tests failed or memory leaks detected!${NC}"
    exit 1
fi
