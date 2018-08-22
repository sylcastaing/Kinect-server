cmake_minimum_required (VERSION 3.2)

project(server)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)

link_directories(/usr/local/lib/)

file(GLOB KINECT_API_SOURCES
"api/KinectApi.h"
"api/KinectApi.cpp"
)

include_directories(api)

set(KINECT_API_SERVER_SOURCES
main.cpp
    ${KINECT_API_SOURCES})

add_executable(kinect_server
${KINECT_API_SERVER_SOURCES})

target_link_libraries(kinect_api_server pistache pthread)