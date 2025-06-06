#!/bin/bash
# Script to format C/C++ code using clang-format with separate style files

# Define directories
SERVER_DIR="server"
GUI_DIR="gui"
LIBZAPPY_NET_DIR="libzappy_net"

# Define C and C++ style files
C_STYLE_FILE="scripts/coding-style/C/.clang-format"
CPP_STYLE_FILE="scripts/coding-style/CPP/.clang-format"

# Create symlinks to the appropriate style files in each directory
echo "Setting up clang-format style links..."
ln -sf "$(pwd)/$C_STYLE_FILE" "$SERVER_DIR/.clang-format"
ln -sf "$(pwd)/$C_STYLE_FILE" "$LIBZAPPY_NET_DIR/.clang-format"
ln -sf "$(pwd)/$CPP_STYLE_FILE" "$GUI_DIR/.clang-format"

# Format each component with appropriate style
echo "Formatting C code (server, libzappy_net)..."
find $SERVER_DIR $LIBZAPPY_NET_DIR -type f \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} \;

echo "Formatting C++ code (gui)..."
find $GUI_DIR -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-format -i {} \;

echo "Code formatting complete."