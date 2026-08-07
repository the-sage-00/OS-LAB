#!/bin/bash
# Q2: Monitor system performance periodically using /proc filesystem

INTERVAL=2
COUNT=5

echo "Monitoring System Performance using /proc"

for ((i=1; i<=COUNT; i++))
do
    echo "========== Sample $i =========="
    echo "Time: $(date)"

    echo "CPU Stats:"
    grep "^cpu " /proc/stat

    echo "Memory Stats:"
    grep -E "MemTotal|MemFree|MemAvailable" /proc/meminfo

    echo "Running Processes:"
    grep "procs_running" /proc/stat

    echo "Uptime:"
    cat /proc/uptime

    echo "==============================="
    sleep $INTERVAL
done
