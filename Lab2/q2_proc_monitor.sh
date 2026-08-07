#!/bin/bash
# ==============================================================================
# OS Lab Assignment 2 — Question 2: /proc Performance Monitoring Script
# ==============================================================================
# This script periodically collects performance metrics directly from the /proc
# virtual filesystem and displays/logs them cleanly.

INTERVAL=${1:-2}      # Sampling interval in seconds (default: 2s)
DURATION=${2:-10}     # Total monitoring duration in seconds (default: 10s)
LOG_FILE="proc_monitor.log"

echo "======================================================================"
echo "          /proc Virtual Filesystem Performance Monitor              "
echo "======================================================================"
echo " Sampling Interval : ${INTERVAL}s"
echo " Total Duration    : ${DURATION}s"
echo " Output Log File   : ${LOG_FILE}"
echo "======================================================================"

# Initialize log file
echo "Timestamp | CPU Usage (%) | Mem Total (MB) | Mem Free (MB) | Mem Avail (MB) | Procs Running | Uptime (s)" > "$LOG_FILE"

elapsed=0
while [ "$elapsed" -lt "$DURATION" ]; do
    TIMESTAMP=$(date "+%Y-%m-%d %H:%M:%S")

    # 1. CPU Usage calculation from /proc/stat
    # Format: cpu user nice system idle iowait irq softirq steal
    CPU_LINE1=$(grep '^cpu ' /proc/stat)
    sleep 0.5
    CPU_LINE2=$(grep '^cpu ' /proc/stat)

    # Parse CPU values
    u1=$(echo "$CPU_LINE1" | awk '{print $2}')
    n1=$(echo "$CPU_LINE1" | awk '{print $3}')
    s1=$(echo "$CPU_LINE1" | awk '{print $4}')
    i1=$(echo "$CPU_LINE1" | awk '{print $5}')
    
    u2=$(echo "$CPU_LINE2" | awk '{print $2}')
    n2=$(echo "$CPU_LINE2" | awk '{print $3}')
    s2=$(echo "$CPU_LINE2" | awk '{print $4}')
    i2=$(echo "$CPU_LINE2" | awk '{print $5}')

    total1=$((u1 + n1 + s1 + i1))
    total2=$((u2 + n2 + s2 + i2))
    idle1=$i1
    idle2=$i2

    total_diff=$((total2 - total1))
    idle_diff=$((idle2 - idle1))

    if [ "$total_diff" -gt 0 ]; then
        CPU_USAGE=$(( 100 * (total_diff - idle_diff) / total_diff ))
    else
        CPU_USAGE=0
    fi

    # 2. Memory Info from /proc/meminfo
    MEM_TOTAL_KB=$(grep 'MemTotal:' /proc/meminfo | awk '{print $2}')
    MEM_FREE_KB=$(grep 'MemFree:' /proc/meminfo | awk '{print $2}')
    MEM_AVAIL_KB=$(grep 'MemAvailable:' /proc/meminfo | awk '{print $2}')

    MEM_TOTAL_MB=$((MEM_TOTAL_KB / 1024))
    MEM_FREE_MB=$((MEM_FREE_KB / 1024))
    MEM_AVAIL_MB=$((MEM_AVAIL_KB / 1024))

    # 3. Running processes & Uptime
    PROCS_RUNNING=$(grep 'procs_running' /proc/stat | awk '{print $2}')
    UPTIME=$(awk '{print $1}' /proc/uptime)

    # Output line
    RECORD="$TIMESTAMP | CPU: ${CPU_USAGE}% | Total Mem: ${MEM_TOTAL_MB}MB | Free Mem: ${MEM_FREE_MB}MB | Avail Mem: ${MEM_AVAIL_MB}MB | Running Procs: ${PROCS_RUNNING} | Uptime: ${UPTIME}s"
    
    echo "$RECORD"
    echo "$RECORD" >> "$LOG_FILE"

    sleep "$INTERVAL"
    elapsed=$((elapsed + INTERVAL))
done

echo "======================================================================"
echo " Monitoring complete. Results logged to ${LOG_FILE}."
echo "======================================================================"
