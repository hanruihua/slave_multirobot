#! /bin/bash
source /opt/ros/kinetic/setup.sh
source ~/catkin_ws/devel/setup.bash

export ROS_MASTER_URI=http://192.168.0.111:11311
export ROS_HOSTNAME=192.168.0.115
export ROS_IP=192.168.0.115
export TURTLEBOT_3D_SENSOR=kinect 
export NAME_SPACE=Slave06
export ROBOT_ID=6

roslaunch --wait slave_bringup multi_minimal_uwb.launch