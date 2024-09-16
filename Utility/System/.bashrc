alias cubash='docker compose exec -it ROS2-Base bash -c "cd workspaces/ROS2-SW && exec bash"'

alias dclean='./Utility/Docker/clean_all'

xhost +local:docker

export CU_USERNAME=op
export CU_UID=$(id -u)
export CU_GID=$(id -g)