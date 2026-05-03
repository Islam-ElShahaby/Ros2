#include "rclcpp/rclcpp.hpp"
#include "rclcpp/node.hpp"
#include "std_msgs/msg/float64.hpp"

#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

class CPUTemperaturePublisher : public rclcpp::Node
{
public:
    CPUTemperaturePublisher() : Node("cpu_temperature_publisher")
    {
        path_ = "/sys/class/hwmon/hwmon4/temp1_input";
        file_.open(path_);
        
        publisher_ = this->create_publisher<std_msgs::msg::Float64>("/cpu_temp", 10);
        timer_ = this->create_wall_timer(1s, std::bind(&CPUTemperaturePublisher::timer_callback, this));
    }
    ~CPUTemperaturePublisher()
    {
        RCLCPP_INFO(this->get_logger(), "Shutting down cpu_temperature_publisher");
        if (file_.is_open())
        {
            file_.close();
        }
    }
private:
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string path_;
    std::ifstream file_;

    double get_cpu_temperature()
    {
        double temperature = 0;
        
        if (!file_.is_open()) {
            file_.open(path_);
        }

        if (file_.is_open()) {
            file_.clear();
            file_.seekg(0);
            if (file_ >> temperature) {
                return temperature / 1000.0;
            }
        }
        RCLCPP_ERROR_ONCE(this->get_logger(), "Failed to read from %s", path_.c_str());
        return -1.0;
    }
    void timer_callback()
    {
        double temperature = get_cpu_temperature();
        auto message = std_msgs::msg::Float64();
        message.data = temperature;
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "CPU Temperature: %f C", temperature);
    }
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CPUTemperaturePublisher>());
    rclcpp::shutdown();
    return 0;
}