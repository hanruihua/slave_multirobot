project(slave_msgs)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  geometry_msgs
  gazebo_msgs
  message_generation
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
)

add_message_files(
  DIRECTORY msg
  FILES
  uwb_position.msg
  robot_state.msg
  # node_frame2.msg
  # vector3d_t.msg
  # system_info.msg
  # node2_t.msg
)


generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
  gazebo_msgs
)

catkin_package(CATKIN_DEPENDS message_runtime)

add_executable(trans2robot_node src/trans2robot.cpp)
target_link_libraries(trans2robot_node ${catkin_LIBRARIES})
# add_dependencies(msg_trans ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(trans2robot_node slave_msgs_generate_messages_cpp)


