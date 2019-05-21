cmake_minimum_required( VERSION 2.8 )
project ( slave_VO )

set( CMAKE_CXX_COMPILER "g++" )
set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_FLAGS "-std=c++11 -march=native -O3" )
#set(OpenCV_DIR "~/source/opencv-4.1.0/OpenCVSDK/usr/local/lib/cmake/opencv4")
set(OpenCV_DIR "/opt/ros/kinetic/share/OpenCV-3.3.1-dev")

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake_modules )
message(Status "123${CMAKE_MODULE_PATH}")
############### dependencies ######################
# Eigen
include_directories( "/usr/include/eigen3" )
# OpenCV
find_package( OpenCV 3.1 REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
# Sophus 
find_package( Sophus REQUIRED )
include_directories( ${Sophus_INCLUDE_DIRS} )
# G2O
find_package( G2O REQUIRED )
include_directories( ${G2O_INCLUDE_DIRS} )

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)

