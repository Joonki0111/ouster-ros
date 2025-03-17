#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

class PointCloudFlipper : public rclcpp::Node {
public:
    PointCloudFlipper() : Node("pointcloud_flipper") {
        subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/sensing/lidar/ouster/points_ex", rclcpp::SensorDataQoS(),
            std::bind(&PointCloudFlipper::pointcloud_callback, this, std::placeholders::_1));
        
        publisher_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/sensing/lidar/ouster/points", rclcpp::SensorDataQoS());
    }

private:
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        // sensor_msgs::msg::PointCloud2 -> pcl::PointCloud 변환
        pcl::PointCloud<pcl::PointXYZ> cloud;
        pcl::fromROSMsg(*msg, cloud);

        // x 좌표 반전
        for (auto &point : cloud.points) {
            point.x *= -1;
        }

        // pcl::PointCloud -> sensor_msgs::msg::PointCloud2 변환
        sensor_msgs::msg::PointCloud2 flipped_msg;
        pcl::toROSMsg(cloud, flipped_msg);
        flipped_msg.header = msg->header;

        publisher_->publish(flipped_msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr publisher_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PointCloudFlipper>());
    rclcpp::shutdown();
    return 0;
}
