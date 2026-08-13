#!/bin/bash
# Question 2: Collect performance monitoring data periodically from /proc

exec >> performance.log
while true
do
    echo "======================================================================"
    echo "Time: $(date)"
    echo "CPU: $(grep "^cpu " /proc/stat)"
    echo "Memory Available: $(grep "^MemAvailable" /proc/meminfo | awk '{print $2, $3}')"
    echo "Load: $(awk '{print $1, $2, $3}' /proc/loadavg)"
    echo "Uptime: $(awk '{print $1}' /proc/uptime) seconds"
    echo "======================================================================"
    sleep 5
done
