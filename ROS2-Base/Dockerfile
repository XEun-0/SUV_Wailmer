ARG BASE_IMAGE=arm64v8/ubuntu:22.04

# Use an appropriate base image
FROM ${BASE_IMAGE}

COPY setup_ros2_base_suite.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/setup_ros2_base_suite.sh

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