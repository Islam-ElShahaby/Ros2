#include "rclcpp/rclcpp.hpp"

class hello_node : public rclcpp::Node {
public:
  hello_node() : rclcpp::Node("new_node") {
    RCLCPP_INFO(this->get_logger(), "Hello, 2😻😻2!");
  }
};

int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<hello_node>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}