# Multiple robots slave
The program of multiple robot project running in slave robot 

Please run the [Master program](https://github.com/hanruihua/master_multirobot) in master computer 

## Environment

- robot -- turtlebot2
- system -- ubuntu 16.06
- UWB -- inffuture
- camera -- kinect v1 or v2
- motion capture -- Optitrack
- platform -- ros kinetic

## Requirement 

#### For turtlebot2
Install the ros package related to turtlebot
> sudo apt-get update
> sudo apt-get install ros-kinetic-turtlebot ros-kinetic-turtlebot-apps ros-kinetic-turtlebot-interactions ros-kinetic-turtlebot-simulator ros-kinetic-kobuki-ftdi ros-kinetic-rocon-* ros-kinetic-ar-track-alvar-msgs

#### For visual odometry(slave_VO)

Eigen
 > sudo apt-get install libeigen3-dev

Sophus
 > git clone https://github.com/strasdat/Sophus.git

OpenCV 3.1
 > in ros library
 
 g2o
 > please use the order version the in https://github.com/hanruihua/slave_multirobot/tree/master/slave_VO/CMake_modules
 
#### For UWB
**Attention**: The serial should have a correct configuration including the byte rate, data, ros publish rate.

serial
> sudo apt-get install ros-kinetic-serial

permission
> sudo chmod 666 /dev/ttyUSB0

## Run
Before run the launch file, you should allocate the "SlaveId" for this slave robot(in .zshrc or .bashrc)
> echo "export NAME_SPACE=SlaveId" >> ~/.zshrc
> source ~/.zshrc 

Run the launch file in each slave robot, make sure the UWB, camera and motion capture are configured correctly

> roslaunch slave_bringup multirobot_bringup.launch

### note(temp)
> git pull in slave_multirobot  
> catkin_make -DCATKIN_BLACKLIST_PACKAGES="slave_vo"  
> cd ~/catkin_ws/src/slave_multirobot/driver  
> sudo sh uwb_serial.sh  (modify bringup)  
 

















