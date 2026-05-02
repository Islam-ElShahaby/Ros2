#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class PublisherA : public rclcpp::Node {
public:
    PublisherA() : Node("publisher_a"), count_(0) {
        publisher_ = this->create_publisher<std_msgs::msg::Int32>("topic_a", 10);
        timer_ = this->create_wall_timer(
            1000ms, std::bind(&PublisherA::timer_callback, this));
    }

private:
    void timer_callback() {
        auto message = std_msgs::msg::Int32();
        message.data = count_++;
        RCLCPP_INFO(this->get_logger(), "Publishing A: '%d'", message.data);
        publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
    int32_t count_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherA>());
    rclcpp::shutdown();
    return 0;
}