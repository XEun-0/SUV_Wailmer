#!/bin/bash

ACTION=$1

BACKUP_DIR="/network_backup"

case "$ACTION" in
backup)
    echo "Backing up current network configuration..."
    mkdir -p $BACKUP_DIR
    cp /etc/network/interfaces $BACKUP_DIR/interfaces.bak
    cp /etc/hostapd/hostapd.conf $BACKUP_DIR/hostapd.conf.bak
    cp /etc/dnsmasq.conf $BACKUP_DIR/dnsmasq.conf.bak
    echo "Backup completed."
    ;;
restore)
    echo "Restoring network configuration..."
    if [ -d "$BACKUP_DIR" ]; then
        cp $BACKUP_DIR/interfaces.bak /etc/network/interfaces
        cp $BACKUP_DIR/hostapd.conf.bak /etc/hostapd/hostapd.conf
        cp $BACKUP_DIR/dnsmasq.conf.bak /etc/dnsmasq.conf
        echo "Restore completed."
    else
        echo "Backup not found. Cannot restore."
    fi
    ;;
*)
    echo "Usage: $0 {backup|restore}"
    ;;
esac

