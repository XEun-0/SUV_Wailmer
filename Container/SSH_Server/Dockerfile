FROM ubuntu:20.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    hostapd \
    dnsmasq \
    iproute2 \
    net-tools \
    iputils-ping \
    sudo

# Copy hostapd and dnsmasq configurations
COPY conf/hostapd.conf /etc/hostapd/hostapd.conf
COPY conf/dnsmasq.conf /etc/dnsmasq.conf
COPY backup_restore.sh /opt/backup_restore.sh

RUN chmod +x /opt/backup_restore.sh

# Backup existing configuration
RUN /opt/backup_restore.sh backup

# Copy the entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

# Specify the default command to run
CMD ["/entrypoint.sh"]
