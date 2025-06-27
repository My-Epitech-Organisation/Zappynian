#!/bin/bash

# Script to test compilation of GUI files individually and create a comprehensive test suite

GUI_DIR="../../gui"
TEST_DIR="."
SUCCESSFUL_FILES=()
FAILED_FILES=()

echo "========================================="
echo "    TESTING INDIVIDUAL FILE COMPILATION"
echo "========================================="

# Test files to compile
FILES_TO_TEST=(
    "App/Game.cpp"
    "Core/EntityManager.cpp"
    "Core/GameState.cpp"
    "Core/WorldScene.cpp"
    "Entities/AEntity.cpp"
    "Entities/Inventory.cpp"
    "Entities/PlayerEntity.cpp"
    "Entities/TileEntity.cpp"
    "Event/EventReceiver.cpp"
    "Network/NetworkManager.cpp"
    "Network/NetworkManagerImpl.cpp"
    "Network/ProtocolParser.cpp"
    "src/main.cpp"
)

# Compiler flags
CXXFLAGS="-Wall -Wextra -std=c++17 -g3 -O0 -fprofile-arcs -ftest-coverage -DTESTING_MODE"
INC_FLAGS="-I$GUI_DIR -I../../libzappy_net/include -I."

echo "Testing compilation of individual GUI files..."
echo ""

# Create temp directory for test objects
mkdir -p temp_test_obj

for file in "${FILES_TO_TEST[@]}"; do
    echo -n "Testing $file ... "
    
    # Try to compile the file
    if g++ $CXXFLAGS $INC_FLAGS -c "$GUI_DIR/$file" -o "temp_test_obj/$(basename $file .cpp).o" 2>/dev/null; then
        echo "✅ SUCCESS"
        SUCCESSFUL_FILES+=("$file")
    else
        echo "❌ FAILED"
        FAILED_FILES+=("$file")
    fi
done

echo ""
echo "========================================="
echo "           COMPILATION RESULTS"
echo "========================================="
echo ""

echo "✅ Successfully compiled files:"
for file in "${SUCCESSFUL_FILES[@]}"; do
    echo "  - $file"
done

echo ""
echo "❌ Failed to compile files:"
for file in "${FAILED_FILES[@]}"; do
    echo "  - $file"
done

echo ""
echo "Success rate: ${#SUCCESSFUL_FILES[@]}/${#FILES_TO_TEST[@]} files"

# Clean up
rm -rf temp_test_obj

echo ""
echo "========================================="
echo "   UPDATING MAKEFILE WITH SUCCESSFUL FILES"
echo "========================================="

# Generate the GUI_SRC_FILES list for Makefile
echo "# GUI source files that compile successfully"
echo "GUI_SRC_FILES = \\"
for i in "${!SUCCESSFUL_FILES[@]}"; do
    if [ $i -eq $((${#SUCCESSFUL_FILES[@]} - 1)) ]; then
        echo "                \$(GUI_DIR)/${SUCCESSFUL_FILES[$i]}"
    else
        echo "                \$(GUI_DIR)/${SUCCESSFUL_FILES[$i]} \\"
    fi
done
