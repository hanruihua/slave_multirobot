add_library( slaveslam SHARED
        frame.cpp
        mappoint.cpp
        map.cpp
        camera.cpp
        config.cpp
        visual_odometry.cpp
        g2o_types.cpp
        )
target_link_libraries(slaveslam ${OpenCV_LIBS} ${Sophus_LIBRARIES} ${G2O_LIBS})
include_directories(${catkin_INCLUDE_DIRS})
