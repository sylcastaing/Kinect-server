cmake_minimum_required (VERSION 3.2)

project(server)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

find_package(libfreenect REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${FREENECT_INCLUDE_DIRS})

file(GLOB KINECT_API_SOURCES
"api/KinectApi.h"
"api/KinectApi.cpp"
"model/Kinect.h"
"model/Kinect.cpp"
)

set(CMAKE_INCLUDE_PATH /usr/local/lib)

set(KINECT_API_SERVER_SOURCES
main.cpp
    ${KINECT_API_SOURCES})

add_executable(kinect_server
${KINECT_API_SERVER_SOURCES})

target_link_libraries(kinect_server pistache pthread freenect freenect_sync ${OpenCV_LIBS})