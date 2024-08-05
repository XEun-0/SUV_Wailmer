#!/bin/bash

cd ROS2
docker build --network=host -f Dockerfile -t ros2 .
cd ../
cd ROS2-Base
docker build --network=host -f Dockerfile -t ros2-base .
cd ../

docker compose up -d
docker compose logs -f ROS2

#winpty docker exec -it ROS2 bash