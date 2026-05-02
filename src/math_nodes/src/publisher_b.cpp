#include <chrono>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class PublisherB : public rclcpp::Node {
public:
    PublisherB() : Node("publisher_b"), count_(100) {
        publisher_ = this->create_publisher<std_msgs::msg::Int32>("topic_b", 10);
        timer_ = this->create_wall_timer(
            1500ms, std::bind(&PublisherB::timer_callback, this));
    }

private:
    void timer_callback() {
        auto message = std_msgs::msg::Int32();
        message.data = count_;
        count_ += 10;
        RCLCPP_INFO(this->get_logger(), "Publishing B: '%d'", message.data);
        publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
    int32_t count_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublisherB>());
    rclcpp::shutdown();
    return 0;
}