#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_srvs/srv/empty.hpp"

using namespace std::chrono_literals;

class PatrolController : public rclcpp::Node {
public:
    PatrolController() : Node("patrol_controller"), is_running(true) {
        this->declare_parameter("linear_speed", 1.5);
        this->declare_parameter("angular_speed", 1.0);

        cmd_vel_publisher = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);

        stop_service = this->create_service<std_srvs::srv::Empty>(
            "/stop", std::bind(&PatrolController::stop_callback, this, std::placeholders::_1, std::placeholders::_2));
        continue_service = this->create_service<std_srvs::srv::Empty>(
            "/continue", std::bind(&PatrolController::continue_callback, this, std::placeholders::_1, std::placeholders::_2));

        timer = this->create_wall_timer(100ms, std::bind(&PatrolController::timer_callback, this));
        
        RCLCPP_INFO(this->get_logger(), "Patrol Controller started. Turtle is running.");
    }

private:
    void stop_callback(const std::shared_ptr<std_srvs::srv::Empty::Request>,
                       std::shared_ptr<std_srvs::srv::Empty::Response>) {
        is_running = false;
        RCLCPP_INFO(this->get_logger(), "Stop service called. Turtle stopping.");
    }

    void continue_callback(const std::shared_ptr<std_srvs::srv::Empty::Request>,
                           std::shared_ptr<std_srvs::srv::Empty::Response>) {
        is_running = true;
        RCLCPP_INFO(this->get_logger(), "Continue service called. Turtle resuming.");
    }

    void timer_callback() {
        auto msg = geometry_msgs::msg::Twist();
        
        if (is_running) {
            msg.linear.x = this->get_parameter("linear_speed").as_double();
            msg.angular.z = this->get_parameter("angular_speed").as_double();
        } else {
            msg.linear.x = 0.0;
            msg.angular.z = 0.0;
        }
        
        cmd_vel_publisher->publish(msg);
    }

    bool is_running;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr stop_service;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr continue_service;
    rclcpp::TimerBase::SharedPtr timer;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PatrolController>());
    rclcpp::shutdown();
    return 0;
}