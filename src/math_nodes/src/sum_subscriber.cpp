#include <memory>
#include <optional>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

class SumSubscriber : public rclcpp::Node {
public:
    SumSubscriber() : Node("sum_subscriber") {
        subscription_a_ = this->create_subscription<std_msgs::msg::Int32>(
            "topic_a", 10, std::bind(&SumSubscriber::callback_a, this, std::placeholders::_1));
        
        subscription_b_ = this->create_subscription<std_msgs::msg::Int32>(
            "topic_b", 10, std::bind(&SumSubscriber::callback_b, this, std::placeholders::_1));

        publisher_sum_ = this->create_publisher<std_msgs::msg::Int32>("topic_sum", 10);
    }

private:
    void callback_a(const std_msgs::msg::Int32::SharedPtr msg) {
        val_a_ = msg->data;
        compute_and_publish();
    }

    void callback_b(const std_msgs::msg::Int32::SharedPtr msg) {
        val_b_ = msg->data;
        compute_and_publish();
    }

    void compute_and_publish() {
        if (val_a_.has_value() && val_b_.has_value()) {
            int32_t sum = val_a_.value() + val_b_.value();
            
            RCLCPP_INFO(this->get_logger(), "A: %d | B: %d | Sum: %d", 
                        val_a_.value(), val_b_.value(), sum);

            auto sum_msg = std_msgs::msg::Int32();
            sum_msg.data = sum;
            publisher_sum_->publish(sum_msg);
        }
    }

    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_a_;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_b_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_sum_;

    std::optional<int32_t> val_a_;
    std::optional<int32_t> val_b_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SumSubscriber>());
    rclcpp::shutdown();
    return 0;
}