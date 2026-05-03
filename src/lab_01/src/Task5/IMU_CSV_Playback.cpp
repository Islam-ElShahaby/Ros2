#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std::chrono_literals;

class IMUCSVPlayback : public rclcpp::Node
{
public:
    IMUCSVPlayback()
        : Node("imu_csv_playback")
    {
        path_ = "/home/mrdark/ros2_ws/src/lab_01/src/Task5/imu_data.csv";
        file_.open(path_);

        if (!file_.is_open()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open %s. Make sure the file exists!", path_.c_str());
        } else {
            std::string header;
            std::getline(file_, header);
        }

        publisher_ = this->create_publisher<sensor_msgs::msg::Imu>("/imu/data", 10);
        timer_ = this->create_wall_timer(
            100ms, std::bind(&IMUCSVPlayback::timer_callback, this));
        RCLCPP_INFO(this->get_logger(), "IMU CSV Playback initialized.");
    }
    ~IMUCSVPlayback()
    {
        RCLCPP_INFO(this->get_logger(), "Shutting down imu_csv_playback");
        if (file_.is_open())
        {
            file_.close();
        }
    }

private:
    void timer_callback()
    {
        if (!file_.is_open()) {
            return;
        }

        std::string line;
        if (!std::getline(file_, line)) {
            // We reached the end of the file. Loop back to start.
            file_.clear(); // Clear EOF flag
            file_.seekg(0); // Rewind
            
            // Read and discard the header again
            std::getline(file_, line);
            
            // Read the first actual data line
            if (!std::getline(file_, line)) {
                return; // File might be empty
            }
        }

        if (line.empty()) return;

        // Parse the CSV line by commas
        std::stringstream ss(line);
        std::string token;
        std::vector<double> values;

        while (std::getline(ss, token, ',')) {
            try {
                values.push_back(std::stod(token));
            } catch (...) {
                values.push_back(0.0);
            }
        }

        // Ensure we have enough columns (assuming 1 timestamp + 10 data columns)
        if (values.size() >= 11) {
            auto message = sensor_msgs::msg::Imu();
            message.header.stamp = this->get_clock()->now();
            message.header.frame_id = "imu";
            
            // values[0] is timestamp, which we ignore and use current time
            message.linear_acceleration.x = values[1];
            message.linear_acceleration.y = values[2];
            message.linear_acceleration.z = values[3];
            message.angular_velocity.x = values[4];
            message.angular_velocity.y = values[5];
            message.angular_velocity.z = values[6];
            message.orientation.x = values[7];
            message.orientation.y = values[8];
            message.orientation.z = values[9];
            message.orientation.w = values[10];

            publisher_->publish(message);
        }
    }

    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string path_;
    std::ifstream file_;
};

int main(int argc, char * argv[])
{
    // Fixed: rclpy -> rclcpp
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<IMUCSVPlayback>());
    rclcpp::shutdown();
    return 0;
}