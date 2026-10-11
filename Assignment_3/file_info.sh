#!/bin/sh

# Question A: Display file information

if [ $# -eq 0 ]; then
    echo "Usage: ./file_info.sh <filename>"
    exit 1
fi

FILE="$1"

if [ ! -e "$FILE" ]; then
    echo "Error: File '$FILE' does not exist."
    exit 1
fi

echo "File Information for: $FILE"
echo "-------------------------------"

echo "Size: $(stat -c '%s' "$FILE") bytes"
echo "Permissions: $(stat -c '%A' "$FILE")"
echo "Owner: $(stat -c '%U' "$FILE")"
echo "Last Modified: $(stat -c '%y' "$FILE")"
echo "File Type: $(stat -c '%F' "$FILE")"

echo "-------------------------------"