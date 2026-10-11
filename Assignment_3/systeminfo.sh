#!/bin/bash

echo "===== SYSTEM INFORMATION ====="

echo
echo "OS Version:"
cat /etc/os-release | grep PRETTY_NAME

echo
echo "Kernel Version:"
uname -r

echo
echo "CPU Information:"
lscpu | grep "Model name"

echo
echo "Memory Information:"
free -h

echo
echo "Disk Usage:"
df -h

echo
echo "Network Information:"
ip addr
