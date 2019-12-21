cd ~/catkin_ws/src/slave_multirobot
git pull
cd ~/catkin_ws
catkin_make -DCATKIN_BLACKLIST_PACKAGES="slave_vo"
source ~/catkin_ws/devel/setup.bash
