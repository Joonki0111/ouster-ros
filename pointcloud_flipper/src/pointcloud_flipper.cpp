#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>

class PointCloudFlipper : public rclcpp::Node {
public:
    PointCloudFlipper() : Node("pointcloud_flipper") {
        subscription_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/sensing/lidar/ouster/points_ex", rclcpp::SensorDataQoS(),
            std::bind(&PointCloudFlipper::pointcloud_callback, this, std::placeholders::_1));

        publisher_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
            "/sensing/lidar/ouster/points", rclcpp::SensorDataQoS());
    }

private:
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        sensor_msgs::msg::PointCloud2 flipped_msg = *msg;
        sensor_msgs::PointCloud2Iterator<float> iter_x(flipped_msg, "x");

        for (; iter_x != iter_x.end(); ++iter_x) {
            *iter_x *= -1;
        }
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
