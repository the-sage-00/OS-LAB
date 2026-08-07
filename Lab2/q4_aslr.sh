#!/bin/bash
# ==============================================================================
# OS Lab Assignment 2 — Question 4: ASLR Management Script & Guide
# ==============================================================================
# Address Space Layout Randomization (ASLR) is a security technique that 
# randomizes memory locations of program components (stack, heap, libraries).

ASLR_FILE="/proc/sys/kernel/randomize_va_space"

echo "======================================================================"
echo "          Address Space Layout Randomization (ASLR) Manager           "
echo "======================================================================"

if [ ! -f "$ASLR_FILE" ]; then
    echo "Error: /proc/sys/kernel/randomize_va_space not found."
    exit 1
fi

CURRENT_VAL=$(cat "$ASLR_FILE")

echo "Current ASLR Status Value: $CURRENT_VAL"
case $CURRENT_VAL in
    0) echo "Status: DISABLED (0) — Memory addresses are static and predictable." ;;
    1) echo "Status: PARTIAL (1)  — Stack, mmap, VDSO randomized." ;;
    2) echo "Status: FULL (2)     — Stack, mmap, VDSO, and Heap (brk) randomized." ;;
esac
echo "----------------------------------------------------------------------"

if [ "$1" == "disable" ]; then
    echo "Attempting to DISABLE ASLR (setting to 0)..."
    echo 0 | sudo tee "$ASLR_FILE" > /dev/null
    echo "New ASLR Status: $(cat $ASLR_FILE)"
elif [ "$1" == "enable" ]; then
    echo "Attempting to ENABLE Full ASLR (setting to 2)..."
    echo 2 | sudo tee "$ASLR_FILE" > /dev/null
    echo "New ASLR Status: $(cat $ASLR_FILE)"
else
    echo "Usage Instructions:"
    echo "  1. Check Status  : ./q4_aslr.sh"
    echo "  2. Disable ASLR  : ./q4_aslr.sh disable   (or: sudo sysctl -w kernel.randomize_va_space=0)"
    echo "  3. Enable ASLR   : ./q4_aslr.sh enable    (or: sudo sysctl -w kernel.randomize_va_space=2)"
    echo "  4. Per-Process Disable without root: setarch \$(uname -m) -R ./your_program"
fi
echo "======================================================================"
