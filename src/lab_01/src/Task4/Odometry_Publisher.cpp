#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"

using namespace std::chrono_literals;

class OdometryPublisher : public rclcpp::Node
{
public:
    OdometryPublisher() : Node("odometry_publisher"), x_pos_(0.0)
    {
        publisher_ = this->create_publisher<nav_msgs::msg::Odometry>("/odom", 10);
        timer_ = this->create_wall_timer(
            100ms, std::bind(&OdometryPublisher::timer_callback, this));
        RCLCPP_INFO(this->get_logger(), "Odometry Publisher initialized.");
    }

private:
    void timer_callback()
    {
        auto message = nav_msgs::msg::Odometry();
        
        message.header.stamp = this->get_clock()->now();
        message.header.frame_id = "odom";
        message.child_frame_id = "base_link";

        message.pose.pose.position.x = x_pos_;
        message.pose.pose.position.y = 0.0;
        message.pose.pose.position.z = 0.0;

        message.pose.pose.orientation.x = 0.0;
        message.pose.pose.orientation.y = 0.0;
        message.pose.pose.orientation.z = 0.0;
        message.pose.pose.orientation.w = 1.0;

        message.twist.twist.linear.x = 1.0;
        message.twist.twist.linear.y = 0.0;
        message.twist.twist.linear.z = 0.0;

        message.twist.twist.angular.x = 0.0;
        message.twist.twist.angular.y = 0.0;
        message.twist.twist.angular.z = 0.0;

        publisher_->publish(message);

        x_pos_ += 0.1;
    }

    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    double x_pos_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OdometryPublisher>());
    rclcpp::shutdown();
    return 0;
}
