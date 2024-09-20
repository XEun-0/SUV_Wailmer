ARG BASE_IMAGE=arm64v8/ros
ARG ROS_DISTRO=iron

# Use an appropriate base image
FROM ${BASE_IMAGE}:${ROS_DISTRO}

USER root

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV SETUP_SCRIPT="setup_ros2_base_suite.sh"
ENV WS_PATH="/workspaces/ROS2-SW"

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
    python3-pip \
    python3-venv \
    python3-tk \
    curl \
    tk \   
    wget \
    figlet \
    && rm -rf /var/lib/apt/lists/* \
    && pip3 install Pillow Flask \
    && wget https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_ARM64.tar.gz \
    && tar -xvzf arduino-cli_latest_Linux_ARM64.tar.gz \
    && mv arduino-cli /usr/local/bin/ \
    && rm arduino-cli_latest_Linux_ARM64.tar.gz \
    && curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py \
    && python3 get-platformio.py

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && usermod -aG dialout $USERNAME \ 
    && apt-get -y install sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME

RUN arduino-cli config init \
    && arduino-cli core update-index \
    && arduino-cli core install arduino:avr \
    && arduino-cli config set library.enable_unsafe_install true \
    && echo "source /opt/ros/iron/setup.bash" >> ~/.bashrc

ENTRYPOINT [ "/usr/local/bin/setup_ros2_base_suite.sh" ]
CMD [ "/bin/bash" ]
