cmake_minimum_required( VERSION 2.8 )
project ( slave_tf )

find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        std_msgs
        geometry_msgs
        message_generation
        )

catkin_package(
        #  INCLUDE_DIRS include
        #  LIBRARIES multi_robot
        CATKIN_DEPENDS roscpp rospy std_msgs message_runtime
        #  DEPENDS system_lib
)



