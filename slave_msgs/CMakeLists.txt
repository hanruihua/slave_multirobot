project(slave_msgs)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  #slave_msgs
  std_msgs
  geometry_msgs
  message_generation
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)

add_message_files(
  DIRECTORY msgs
  FILES
  slave_VelocityID.msg
  uwb_position.msg
)


generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(CATKIN_DEPENDS message_runtime)

add_executable(msg_trans src/msg_trans_node.cpp)
target_link_libraries(msg_trans ${catkin_LIBRARIES})
#add_dependencies(msg_trans ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(msg_trans slave_msgs_generate_messages_cpp)


