#!/bin/bash

if [ "$1" == "restore" ]; then
    /opt/backup_restore.sh restore
    exit 0
fi

# Assign a static IP to wlan0
ip addr add 192.168.4.1/24 dev wlan0

# Enable IP forwarding
echo 1 > /proc/sys/net/ipv4/ip_forward

# Start hostapd and dnsmasq
systemctl start hostapd
systemctl start dnsmasq

# Keep the container running
tail -f /dev/null

