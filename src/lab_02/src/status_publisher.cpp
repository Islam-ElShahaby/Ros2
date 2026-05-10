#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "lab_02/msg/robot_status.hpp"
#include <cmath>

using namespace std::chrono_literals;

class StatusPublisher : public rclcpp::Node {
public:
    StatusPublisher() : Node("status_publisher"), total_theta(0.0), first_pose_received(false), current_state("running") {
        this->declare_parameter("status_rate", 5.0);
        double rate = this->get_parameter("status_rate").as_double();

        status_publisher = this->create_publisher<lab_02::msg::RobotStatus>("/robot/status", 10);
        
        pose_subscriber = this->create_subscription<turtlesim::msg::Pose>(
            "/turtle1/pose", 10, std::bind(&StatusPublisher::pose_callback, this, std::placeholders::_1));
            
        cmd_vel_subscriber = this->create_subscription<geometry_msgs::msg::Twist>(
            "/turtle1/cmd_vel", 10, std::bind(&StatusPublisher::cmd_vel_callback, this, std::placeholders::_1));

        auto timer_period = std::chrono::duration<double>(1.0 / rate);
        timer = this->create_wall_timer(
            std::chrono::duration_cast<std::chrono::milliseconds>(timer_period),
            std::bind(&StatusPublisher::timer_callback, this));
            
        RCLCPP_INFO(this->get_logger(), "Status Publisher started at %.1f Hz", rate);
    }

private:
    void pose_callback(const turtlesim::msg::Pose::SharedPtr msg) {
        current_pose = *msg;

        if (!first_pose_received) {
            last_theta = msg->theta;
            first_pose_received = true;
            return;
        }

        double delta_theta = msg->theta - last_theta;
        
        if (delta_theta > M_PI) {
            delta_theta -= 2.0 * M_PI;
        } else if (delta_theta < -M_PI) {
            delta_theta += 2.0 * M_PI;
        }

        total_theta += delta_theta;
        last_theta = msg->theta;
    }

    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg) {
        if (msg->linear.x == 0.0 && msg->angular.z == 0.0) {
            current_state = "stopped";
        } else {
            current_state = "running";
        }
    }

    void timer_callback() {
        if (!first_pose_received) return;

        auto status_msg = lab_02::msg::RobotStatus();
        
        status_msg.pose.x = current_pose.x;
        status_msg.pose.y = current_pose.y;
        status_msg.pose.theta = current_pose.theta;

        status_msg.state = current_state;

        int lap_count = std::abs(total_theta / (2.0 * M_PI));
        status_msg.lap_count = lap_count;
        status_msg.temperature = 30.0f + (lap_count * 1.5f);

        status_publisher->publish(status_msg);
    }

    turtlesim::msg::Pose current_pose;
    double last_theta;
    double total_theta;
    bool first_pose_received;
    std::string current_state;

    rclcpp::Publisher<lab_02::msg::RobotStatus>::SharedPtr status_publisher;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscriber;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscriber;
    rclcpp::TimerBase::SharedPtr timer;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StatusPublisher>());
    rclcpp::shutdown();
    return 0;
}