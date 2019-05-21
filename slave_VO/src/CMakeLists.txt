add_library( slaveslam SHARED
        frame.cpp
        mappoint.cpp
        map.cpp
        camera.cpp
        config.cpp
        visual_odometry.cpp
        )

target_link_libraries(slaveslam ${OpenCV_LIBS} ${Sophus_LIBRARIES})
