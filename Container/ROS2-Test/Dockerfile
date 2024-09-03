ARG BASE_IMAGE=ros
ARG ROS_DISTRO=iron

# Use an appropriate base image
FROM ${BASE_IMAGE}:${ROS_DISTRO}

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

COPY setup_ros2_base_suite.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/setup_ros2_base_suite.sh

RUN ls
# Install dependecies
RUN apt-get update && apt-get install -y \
    # Non-arm64
    curl \
    hostapd \
    dnsmasq \
    iproute2 \
    iptables \
    net-tools \
    wireless-tools \
    wget \
    figlet \
    # Non-arm64
    && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh \
    && arduino-cli config init \
    && arduino-cli core update-index \
    && arduino-cli core install arduino:avr \
    && arduino-cli config set library.enable_unsafe_install true \
    && echo "source /opt/ros/iron/setup.bash" >> ~/.bashrc


ENTRYPOINT [ "/usr/local/bin/setup_ros2_base_suite.sh" ]
CMD [ "/bin/bash" ]