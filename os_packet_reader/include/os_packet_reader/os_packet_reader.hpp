#include <rclcpp/rclcpp.hpp>
#include "ouster_sensor_msgs/msg/packet_msg.hpp"
#include "std_msgs/msg/string.hpp"

#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 22100

struct OS1_datablock
{
    unsigned int range;
    unsigned short reflectivity;
    unsigned short signal;
    unsigned short nir;
    unsigned short reserved;
};

#pragma pack(push, 1)
struct OS1_column
{
    unsigned long long timestamp;
    unsigned short measurement_id;
    unsigned short frame_id;
    unsigned int encoder_count;
    OS1_datablock datablock[128];
    unsigned int block_status;
};
#pragma pack(pop)

struct OS1_packet
{
    OS1_column column[16];
};

class OSPacketReader : public rclcpp::Node
{
    public:
        explicit OSPacketReader();

    private:
        rclcpp::Publisher<ouster_sensor_msgs::msg::PacketMsg>::SharedPtr lidar_packet_pub_;
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr metadata_pub_;
        
        rclcpp::TimerBase::SharedPtr timer_;

        ouster_sensor_msgs::msg::PacketMsg lidar_packet_;

        int sockfd_;
        sockaddr_in servaddr_, cliaddr_;
        socklen_t len_;
        int n_;
        OS1_packet packet_;
        std::thread recv_thread_;
        
        void timerCallback();
        std::string ReadMetadata();
};

