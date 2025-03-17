cmake_minimum_required(VERSION 3.8)
project(pointcloud_flipper)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENTS common io)

add_executable(pointcloud_flipper src/pointcloud_flipper.cpp)

ament_target_dependencies(pointcloud_flipper rclcpp sensor_msgs pcl_conversions PCL)

install(TARGETS pointcloud_flipper
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
