cmake_minimum_required(VERSION 3.8)
project(os_packet_reader)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ouster_sensor_msgs REQUIRED)
find_package(std_msgs REQUIRED)

include_directories(include)

add_executable(os_packet_reader_node
  src/os_packet_reader_node.cpp
  src/os_packet_reader.cpp
)

ament_target_dependencies(os_packet_reader_node
  rclcpp
  std_msgs
  ouster_sensor_msgs
)

install(TARGETS
  os_packet_reader_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
