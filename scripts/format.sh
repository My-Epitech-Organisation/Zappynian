#!/bin/bash
# Script to format C/C++ code using clang-format
find server gui common -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" \) -exec clang-format -style=file -i {} \;