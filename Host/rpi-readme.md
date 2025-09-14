Host computer 
export DISPLAY=localhost:0.0
sudo apt install x11-xserver-utils
xhost +local:root
xhost +local:dev




ssh into rpi using ssh -Y <username>@<rpi-ip>
<!-- sudo apt install x11-apps -->

sudo apt install -y xauth x11-apps libx11-6 libxcb-xinerama0 libxrender1 libxext6