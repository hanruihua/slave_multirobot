#! /bin/bash

source /opt/ros/kinetic/setup.sh
source ~/catkin_ws/devel/setup.bash

cd ~/catkin_ws/src/slave_multirobot
sleep 5
git pull
cd ~/catkin_ws
catkin_make -DCATKIN_BLACKLIST_PACKAGES="slave_vo"



source $(locate $1)

env | grep NAME_SPACE

roslaunch --wait slave_bringup multi_minimal_uwb.launch
