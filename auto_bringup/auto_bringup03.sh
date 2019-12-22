#! /bin/bash
source /opt/ros/kinetic/setup.sh

source /home/turtlebot/catkin_ws/devel/setup.bash

export ROS_MASTER_URI=http://192.168.0.111:11311
export ROS_HOSTNAME=192.168.0.113
export ROS_IP=192.168.0.113
export TURTLEBOT_3D_SENSOR=kinect
export NAME_SPACE=Slave03
export ROBOT_ID=3

roslaunch --wait slave_bringup multi_minimal_uwb.launch
