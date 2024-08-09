# Use an appropriate base image
FROM ubuntu:20.04

COPY setup_ros2_suite.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/setup_ros2_suite.sh

#install dependecies
RUN apt-get update && apt-get install -y \
    hostapd \
    dnsmasq \
    iproute2 \
    iptables \
    net-tools \
    wireless-tools

ENTRYPOINT [ "/usr/local/bin/setup_ros2_suite.sh" ]
CMD [ "/bin/bash" ]