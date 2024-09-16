#!/bin/bash

# SUV_NAME=SUV_WAILMER
# WORK_DIR=workspaces/ROS2-SW/
# ARD_LIB_DIR=Devices/Arduino/Libraries/
# ROS_WS_DIR=ROS_ws/
#
# echo "[ $SUV_NAME ] has started"
#
# figlet -c Suv
#
# cd ${WORK_DIR}
#
# pushd ${ARD_LIB_DIR}
# echo "[ $SUV_NAME ] has initiated arduino-cli library installation.."
# ./lib_install.sh
# echo "[ $SUV_NAME ] has finished arduino-cli library installation"
# popd
#
# pushd ${ROS_WS_DIR}
# echo "[ $SUV_NAME ] has initiated ROS2 package compilation.."
# ./build_ros_pkgs
# echo "[ $SUV_NAME ] has finished ROS2 package compilation"
# popd

exec "$@"

