#include "os_packet_reader/os_packet_reader.hpp"

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<OSPacketReader>();
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
