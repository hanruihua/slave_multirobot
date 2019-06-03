cmake_minimum_required(VERSION 2.8.3)
project(slave_uwb)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  serial
  message_generation
  genmsg
  geometry_msgs
  slave_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES uwb
   CATKIN_DEPENDS roscpp rospy std_msgs serial message_runtime geometry_msgs
#  DEPENDS system_lib
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(slave_uwb src/uwb_node.cpp)
#add_dependencies(slave_uwb ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS} )
add_dependencies(slave_uwb slave_msgs_generate_messages_cpp)
target_link_libraries(slave_uwb ${catkin_LIBRARIES})
 

