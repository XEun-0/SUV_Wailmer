#!/bin/bash
echo "Start [ setup_ros2_suite.sh ]"

# testing code:
# sudo chmod -R 777 ./
# make -f Makefile
# MAKE_STATUS=$?

# STATUS_FILE="/tmp/entrypoint_done"
# echo $MAKE_STATUS > $STATUS_FILE

exec "$@"