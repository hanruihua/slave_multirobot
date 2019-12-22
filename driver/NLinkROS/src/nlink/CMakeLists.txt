cmake_minimum_required(VERSION 2.8.3)
project(nlink)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    std_msgs
    geometry_msgs
    message_generation
    )


## Generate messages in the 'msg' folder
add_message_files(
    FILES
    vector3d_t.msg
    system_info.msg
    tag_frame0.msg
    tag_to_anchor_dis_t.msg
    tag_t.msg
    anchor_frame0.msg
    node0_t.msg
    node_frame0.msg
    node1_t.msg
    node_frame1.msg
    node2_t.msg
    node_frame2.msg
    tofsense_frame0.msg
    tofsense_cascade.msg
    )

## Generate added messages and services with any dependencies listed here
generate_messages(
    DEPENDENCIES
    std_msgs
    geometry_msgs
    )

catkin_package(
    #  INCLUDE_DIRS include
    #  LIBRARIES LinkTrack
    #  CATKIN_DEPENDS roscpp
    #  DEPENDS system_lib
    CATKIN_DEPENDS message_runtime
    )

include_directories(
    include ${catkin_INCLUDE_DIRS}
    )

add_executable(linktrack_publisher
    src/serial/serial.cpp
    src/serial/impl/unix.cpp
    src/serial/impl/win.cpp
    src/serial/impl/list_ports/list_ports_linux.cpp
    src/serial/initserial.cpp

    src/NProtocol/ncommon.cpp
    src/NProtocol/nframeextraction.cpp
    src/NProtocol/nprotocolbase.cpp
    src/NProtocol/nprotocolitembase.cpp
    src/NProtocol/nprotocolitems.cpp

    src/NLink/ndatatype.cpp
    src/NLink/nsysteminfo.cpp

    src/NLink/LinkTrack/ncommon.cpp
    src/NLink/LinkTrack/nparameter.cpp
    src/NLink/LinkTrack/nanchorframe0.cpp
    src/NLink/LinkTrack/ntagframe0.cpp
    src/NLink/LinkTrack/nnodeframe0.cpp
    src/NLink/LinkTrack/nnodeframe1.cpp
    src/NLink/LinkTrack/nnodeframe2.cpp

    src/LinkTrack/init.cpp
    src/LinkTrack/publisher.cpp
    )

target_link_libraries(linktrack_publisher
    ${catkin_LIBRARIES}
    )
add_dependencies(linktrack_publisher nlink_generate_messages_cpp)


add_executable(tofsense_publisher
    src/serial/serial.cpp
    src/serial/impl/unix.cpp
    src/serial/impl/win.cpp
    src/serial/impl/list_ports/list_ports_linux.cpp
    src/serial/initserial.cpp

    src/NProtocol/ncommon.cpp
    src/NProtocol/nframeextraction.cpp
    src/NProtocol/nprotocolbase.cpp
    src/NProtocol/nprotocolitembase.cpp
    src/NProtocol/nprotocolitems.cpp

    src/NLink/ndatatype.cpp
    src/NLink/nsysteminfo.cpp

    src/NLink/TOFSense/ncommon.cpp
    src/NLink/TOFSense/nframe0.cpp
    src/NLink/TOFSense/nreadframe0.cpp

    src/TOFSense/init.cpp
    src/TOFSense/publisher.cpp
    )

target_link_libraries(tofsense_publisher
    ${catkin_LIBRARIES}
    )
add_dependencies(tofsense_publisher nlink_generate_messages_cpp)
