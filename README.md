# Multiple robots slave
The program of multiple robot project running in slave robot 

Please run the [Master program](https://github.com/hanruihua/master_multirobot) in master computer 

## Test Environment

- robot -- turtlebot2
- system -- ubuntu 16.04 
- UWB -- nooploop
- camera -- kinect v1 or v2
- motion capture -- Optitrack
- platform -- ros kinetic/ melodic

## Installation

> cd ~/catkin_ws/src
> git clone https://github.com/hanruihua/slave_multirobot.git

### For turtlebot2

Install the ros package related to turtlebot
> sudo apt-get update  
> sudo apt-get install ros-kinetic-turtlebot ros-kinetic-turtlebot-apps ros-kinetic-turtlebot-interactions ros-kinetic-turtlebot-simulator ros-kinetic-kobuki-ftdi ros-kinetic-rocon-* ros-kinetic-ar-track-alvar-msgs  
### For visual odometry(slave_VO)

Eigen
 > sudo apt-get install libeigen3-dev

Sophus
 > git clone https://github.com/strasdat/Sophus.git

OpenCV 3.1
 > in ros library
 
 g2o
 > please use the order version the in https://github.com/hanruihua/slave_multirobot/tree/master/slave_VO/CMake_modules
 
### For UWB (nlink)

**Attention**: The byte rate of serial should equal to the ros loop rate when you want read the serial data(when you use the slave_uwb node). If you use the modified nlink uwb node, you can ignore this ros loop rate requirements.

serial
> sudo apt-get install ros-kinetic-serial  

modify serial rule
> cd slave_driver/nlink  
> sudo sh uwb_serial.sh  

### Configure ros network and environment parameter
For version 1:
Before run the launch file, you should allocate the "SlaveID"(Slave01 Slave02 ...) for this slave robot(in .zshrc or .bashrc)

template:

> echo "export ROS_MASTER_URI=http://master_ip:11311" >> ~/.zshrc   
> echo "export ROS_HOSTNAME=localhost_ip" >> ~/.zshrc  
> echo "export ROS_IP=localhost_ip" >> ~/.zshrc   
> echo "export TURTLEBOT_3D_SENSOR=kinect" >> ~/.zshrc  
> echo "export NAME_SPACE=SlaveID" >> ~/.zshrc
> echo "export ROBOT_ID=id" >> ~/.zshrc

> source ~/.zshrc  

example:
- In .zshrc or .bashrc:

    - export ROS_MASTER_URI=http://192.168.0.111:11311  
    - export ROS_HOSTNAME=192.168.0.114  
    - export ROS_IP=192.168.0.114  
    - export TURTLEBOT_3D_SENSOR=kinect  
    - export NAME_SPACE=Slave07
    - export ROBOT_ID=7

For version 2:
Skip this part.
## Build
For version 1:
> cd ~/catkin_ws
> catkin_build

If you do not need certain package, you can use catkin_make -DCATKIN_BLACKLIST_PACKAGES="floder" to ignore. 

example: 
     catkin_make -DCATKIN_BLACKLIST_PACKAGES="slave_vo"

For version 2:
Skip this part.
## Run
For version 1:
Run the launch file in each slave robot to launch each turtlebot
> roslaunch slave_bringup multi_minimal.launch

For version 2: 
**Note** In version 2, we provide the configure files. In this version, the SlaveID is changed to agentID, we don't need the ROBOT_ID anymore.

You can skip the configure, build and run parts, simply add the auto_bringup.sh to the startup application by 

>gnome-terminal -x /home/turtlebot/catkin_ws/src/slave_multirobot/slave_bringup/auto_bringup/auto_bringup.sh 'my_inc01'
>sudo updatedb

Then, when the robot startup, it will automatically pull the latest code, build it and run the launch file.

### note(temp)
> git pull in slave_multirobot  
> catkin_make -DCATKIN_BLACKLIST_PACKAGES="slave_vo"  
> cd ~/catkin_ws/src/slave_multirobot/driver  
 
 

















