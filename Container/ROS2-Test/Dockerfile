ARG BASE_IMAGE=ros
ARG ROS_DISTRO=iron

# Use an appropriate base image
FROM ${BASE_IMAGE}:${ROS_DISTRO}

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# COPY setup_ros2_base_suite.sh /usr/local/bin/
# RUN chmod +x /usr/local/bin/setup_ros2_base_suite.sh

# #install dependecies
RUN apt-get update && apt-get install -y \
    hostapd \
    dnsmasq \
    iproute2 \
    iptables \
    net-tools \
    wireless-tools \
    wget \
    && wget https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_ARM64.tar.gz \
    && tar -xvzf arduino-cli_latest_Linux_ARM64.tar.gz \
    && mv arduino-cli /usr/local/bin/ \
    && rm arduino-cli_latest_Linux_ARM64.tar.gz \
    && arduino-cli config init \
    && arduino-cli core update-index \
    && arduino-cli core install arduino:avr


# ENTRYPOINT [ "/usr/local/bin/setup_ros2_base_suite.sh" ]
CMD [ "/bin/bash" ]
