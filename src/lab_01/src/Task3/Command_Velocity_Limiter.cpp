#include <algorithm>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class CommandVelocityLimiter : public rclcpp::Node
{
public:
    CommandVelocityLimiter() : Node("command_velocity_limiter")
    {
        subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel", 10, std::bind(&CommandVelocityLimiter::topic_callback, this, std::placeholders::_1));
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel_limited", 10);
        RCLCPP_INFO(this->get_logger(), "Command Velocity Limiter initialized.");
    }

private:
    void topic_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const
    {
        auto limited_msg = *msg;
        bool limited = false;

        if (std::abs(msg->linear.x) > 1.0) {
            limited_msg.linear.x = std::clamp(msg->linear.x, -1.0, 1.0);
            limited = true;
        }
        
        if (std::abs(msg->angular.z) > 1.5) {
            limited_msg.angular.z = std::clamp(msg->angular.z, -1.5, 1.5);
            limited = true;
        }

        if (limited) {
            RCLCPP_WARN(this->get_logger(), 
                "Velocity limits exceeded! Limiting linear speed to 1.0 m/s and angular velocity to 1.5 rad/s.");
        }

        publisher_->publish(limited_msg);
    }

    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CommandVelocityLimiter>());
    rclcpp::shutdown();
    return 0;
}