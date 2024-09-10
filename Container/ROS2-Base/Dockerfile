ARG BASE_IMAGE=arm64v8/ros
ARG ROS_DISTRO=iron

# Use an appropriate base image
FROM ${BASE_IMAGE}:${ROS_DISTRO}

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV SETUP_SCRIPT="setup_ros2_base_suite.sh"

COPY ${SETUP_SCRIPT} /usr/local/bin/
RUN chmod +x /usr/local/bin/${SETUP_SCRIPT}

# Install dependecies
RUN apt-get update && apt-get install -y \
    hostapd \
    dnsmasq \
    iproute2 \
    iptables \
    net-tools \
    wireless-tools \
    python3-opencv \
    libopencv-dev \   
    wget \
    figlet \
    && rm -rf /var/lib/apt/lists/* \
    && wget https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_ARM64.tar.gz \
    && tar -xvzf arduino-cli_latest_Linux_ARM64.tar.gz \
    && mv arduino-cli /usr/local/bin/ \
    && rm arduino-cli_latest_Linux_ARM64.tar.gz \
    && arduino-cli config init \
    && arduino-cli core update-index \
    && arduino-cli core install arduino:avr \
    && arduino-cli config set library.enable_unsafe_install true \
    && echo "source /opt/ros/iron/setup.bash" >> ~/.bashrc

ENTRYPOINT [ "/usr/local/bin/setup_ros2_base_suite.sh" ]
CMD [ "/bin/bash" ]
