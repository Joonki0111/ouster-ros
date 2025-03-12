#include "os_packet_reader/os_packet_reader.hpp"

OSPacketReader::OSPacketReader() : Node("os_packet_reader_node")
{
    sockaddr_in servaddr; 

    if ((sockfd_ = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    }

    int buffer_size = 100 * 1024 * 1024;
    setsockopt(sockfd_, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));

    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr_, 0, sizeof(cliaddr_)); 
    
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd_, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 

    len_ = sizeof(cliaddr_);
    lidar_packet_.buf.resize(sizeof(OS1_packet));

    auto metadata_qos = rclcpp::QoS(rclcpp::KeepLast(1));
    metadata_qos.reliability(RMW_QOS_POLICY_RELIABILITY_RELIABLE);
    metadata_qos.durability(RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL);

    lidar_packet_pub_ = create_publisher<ouster_sensor_msgs::msg::PacketMsg>("/ouster/lidar_packets", rclcpp::QoS(1));
    imu_packet_pub_ = create_publisher<ouster_sensor_msgs::msg::PacketMsg>("/ouster/imu_packets", rclcpp::QoS(1));
    metadata_pub_ = create_publisher<std_msgs::msg::String>("/ouster/metadata", metadata_qos);
    
    timer_ = create_wall_timer(std::chrono::microseconds(1500), std::bind(&OSPacketReader::timerCallback, this)); // 640Hz

    std_msgs::msg::String metadata;
    metadata.data = ReadMetadata();
    metadata_pub_->publish(metadata);
}

void OSPacketReader::timerCallback()
{
    OS1_packet packet;
    recvfrom(sockfd_, &packet, sizeof(OS1_packet), 0, (struct sockaddr *)&cliaddr_, &len_);
	std::memcpy(lidar_packet_.buf.data(), &packet, sizeof(packet));
    lidar_packet_pub_->publish(lidar_packet_);
}

std::string OSPacketReader::ReadMetadata()
{
    std::string s;
    std::string sTotal;

    std::ifstream in;
    in.open("/home/sws/169.254.174-metadata.json");

    while(!in.eof()) 
    {
        std::getline(in, s);
        sTotal += s + "\n";
    }

    in.close();
    return sTotal;
}