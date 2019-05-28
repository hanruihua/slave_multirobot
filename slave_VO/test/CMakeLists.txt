#cmake_minimum_required(VERSION 2.8.7)
#project(slave_vo)
#ros
#include_directories(${catkin_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include )
message(Status "123${catkin_INCLUDE_DIRS}")
add_executable( run_vo run_vo.cpp )
target_link_libraries( run_vo slaveslam ${catkin_LIBRARIES})




