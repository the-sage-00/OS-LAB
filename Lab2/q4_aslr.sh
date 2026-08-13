#!/bin/bash
# Question 4: Enable and Disable ASLR (Address Space Layout Randomization)

ASLR_FILE="/proc/sys/kernel/randomize_va_space"

echo "Checking ASLR Status..."
if [ -f "$ASLR_FILE" ]; then
    VAL=$(cat "$ASLR_FILE")
    echo "Current /proc/sys/kernel/randomize_va_space value: $VAL"
    if [ "$VAL" -eq 0 ]; then
        echo "ASLR is DISABLED."
    elif [ "$VAL" -eq 1 ]; then
        echo "ASLR is PARTIALLY ENABLED (Stack, VDSO, Shared Memory)."
    elif [ "$VAL" -eq 2 ]; then
        echo "ASLR is FULLY ENABLED (Stack, Heap, VDSO, Shared Memory)."
    fi
else
    echo "ASLR configuration file not found."
fi

echo ""
echo "Commands to change ASLR status:"
echo "Disable ASLR: echo 0 | sudo tee /proc/sys/kernel/randomize_va_space"
echo "Enable ASLR:  echo 2 | sudo tee /proc/sys/kernel/randomize_va_space"
echo "Run binary without ASLR (per-process): setarch \$(uname -m) -R ./your_program"
