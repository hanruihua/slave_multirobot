add_library( slaveslam SHARED
        frame.cpp
        mappoint.cpp
        map.cpp
        camera.cpp
        config.cpp
        visual_odometry.cpp
        g2o_types.cpp
        )
target_link_libraries(slaveslam ${OpenCV_LIBS} ${Sophus_LIBRARIES} g2o_core g2o_stuff g2o_types_sba)
include_directories(${catkin_INCLUDE_DIRS})
