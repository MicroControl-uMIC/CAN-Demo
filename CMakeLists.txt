cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_SYSROOT /opt/umic200/sysroot)
set(CMAKE_STAGING_PREFIX /home/umic)


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

include_directories(SYSTEM ./source)


project(can-demo VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#--------------------------------------------------------------------
# Comment the following line to get debug messages using qDebug()
# 
add_definitions(-DQT_NO_DEBUG_OUTPUT)


find_package(Qt5 COMPONENTS Core Network REQUIRED)


add_executable(${PROJECT_NAME} source/can_demo.cpp source/qcan_socket.cpp)
target_link_libraries(${PROJECT_NAME} QCANopenMaster Qt5::Core Qt5::Network)
