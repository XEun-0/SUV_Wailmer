ARG BASE_IMAGE=ros
ARG ROS_DISTRO=iron

# Use an appropriate base image
FROM ${BASE_IMAGE}:${ROS_DISTRO}

USER root

ARG USERNAME=dev
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV SETUP_SCRIPT="setup_ros2_test_suite.sh"
ENV WS_PATH="/workspaces/ROS2-SW"

# Copy the setup script into the container
COPY ${SETUP_SCRIPT} /usr/local/bin/
RUN chmod +x /usr/local/bin/${SETUP_SCRIPT}

# Install dependencies
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
    qtbase5-dev \
    qttools5-dev-tools \
    qtchooser \
    qt5-qmake \
    libqt5widgets5 \
    libqt5gui5 \
    libqt5core5a \
    build-essential \
    cmake \
    g++ \
    libgl1-mesa-dev \
    libx11-dev \
    libxext-dev \
    libxi-dev \
    libxrender-dev \
    libxcb1-dev \
    libx11-xcb-dev \
    libglu1-mesa-dev \
    x11-xserver-utils \
    x11-apps \
    sudo \
    && curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh \
    && rm -rf /var/lib/apt/lists/*

# Create a new user with sudo privileges
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && usermod -aG dialout $USERNAME \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# Switch to non-root user
USER $USERNAME

# Arduino CLI setup
RUN arduino-cli config init \
    && arduino-cli core update-index \
    && arduino-cli core install arduino:avr \
    && arduino-cli config set library.enable_unsafe_install true \
    && echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> ~/.bashrc

# Set entrypoint and default command
ENTRYPOINT [ "/usr/local/bin/setup_ros2_test_suite.sh" ]
CMD [ "/bin/bash" ]
