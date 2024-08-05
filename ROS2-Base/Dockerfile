ARG BASE_IMAGE=arm64v8/ubuntu:22.04

# Use an appropriate base image
FROM ${BASE_IMAGE}

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=iron

COPY setup_ros2_base_suite.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/setup_ros2_base_suite.sh

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-colcon-common-extensions \
    python3-flake8 \
    python3-pip \
    python3-pytest \
    python3-setuptools \
    python3-vcstool \
    wget \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
    libasio-dev \
    libtinyxml2-dev \
    && rm -rf /var/lib/apt/lists/*

# Create workspace
RUN mkdir -p /workspace/src
WORKDIR /workspace

# Clone ROS 2 repositories
RUN wget https://raw.githubusercontent.com/ros2/ros2/iron/ros2.repos && \
    vcs import src < ros2.repos

# Install ROS 2 dependencies using rosdep
RUN apt-get update && apt-get install -y \
    python3-rosdep \
    && rosdep init && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -y

# Build ROS 2
COPY ros2_colcon_defaults.yaml /workspace/
RUN colcon build --defaults-file /workspace/ros2_colcon_defaults.yaml

# Source the environment
RUN echo "source /workspace/install/setup.bash" >> ~/.bashrc


# #install dependecies
# RUN apt-get update && apt-get install -y \
#     hostapd \
#     dnsmasq \
#     iproute2 \
#     iptables \
#     net-tools \
#     wireless-tools

ENTRYPOINT [ "/usr/local/bin/setup_ros2_base_suite.sh" ]
CMD [ "/bin/bash" ]