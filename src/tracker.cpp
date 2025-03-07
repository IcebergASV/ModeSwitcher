#include "rclcpp/rclcpp.hpp"
#include "mavros_msgs/msg/waypoint_reached.hpp"
#include "mavros_msgs/srv/set_mode.hpp"  // Include the service definition
#include <iostream>
#include <cstdlib> // For std::atoi

class WaypointCounter : public rclcpp::Node
{
public:
    WaypointCounter(int number) : Node("tracker"), target_wp_(number), wp_completed_count_(0)
    {
        RCLCPP_INFO(this->get_logger(), "Total Waypoints: %d", target_wp_);

        // Create the service client for /mavros/set_mode
        mode_change_client_ = this->create_client<mavros_msgs::srv::SetMode>("/mavros/set_mode");

        // Subscribe to MAVROS waypoint reached topic
        wp_reached_sub_ = this->create_subscription<mavros_msgs::msg::WaypointReached>(
            "/mavros/mission/reached", 10, 
            std::bind(&WaypointCounter::wpReachedCallback, this, std::placeholders::_1));
    }

private:
    // Callback function triggered when a waypoint is reached
    void wpReachedCallback(const mavros_msgs::msg::WaypointReached::SharedPtr msg)
    {
        wp_completed_count_++; // Increment count on waypoint reached

        RCLCPP_INFO(this->get_logger(), "Waypoint Reached: %d, Total Completed: %d", msg->wp_seq, wp_completed_count_);

        // Compare wp_seq with the input number
        if (wp_completed_count_ >= target_wp_)
        {
            RCLCPP_INFO(this->get_logger(), "Switching to Guided");
            switchToGuided();
        }
    }

    void switchToGuided()
    {
        // Create request
        auto request = std::make_shared<mavros_msgs::srv::SetMode::Request>();
        //request->base_mode = 0;  // Not used in ROS 2
        request->custom_mode = "GUIDED";

        // Send request asynchronously
        auto future = mode_change_client_->async_send_request(request, 
            [this](rclcpp::Client<mavros_msgs::srv::SetMode>::SharedFuture response) {
                try {
                    if (response.get()->mode_sent) {
                        RCLCPP_INFO(this->get_logger(), "Successfully set mode to GUIDED");
                    } else {
                        RCLCPP_WARN(this->get_logger(), "Failed to set mode to GUIDED");
                    }
                } catch (const std::exception& e) {
                    RCLCPP_ERROR(this->get_logger(), "Service call failed: %s", e.what());
                }
    });
    }

    int target_wp_;  // The input waypoint number to compare against
    int wp_completed_count_;  // Stores the count of waypoints reached
    rclcpp::Subscription<mavros_msgs::msg::WaypointReached>::SharedPtr wp_reached_sub_;
    rclcpp::Client<mavros_msgs::srv::SetMode>::SharedPtr mode_change_client_;
};


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    if (argc < 2)
    {
        std::cerr << "Usage: ros2 run my_package waypoint_counter <integer>\n";
        return 1;
    }

    int input_number = std::atoi(argv[1]);

    rclcpp::spin(std::make_shared<WaypointCounter>(input_number));

    rclcpp::shutdown();
    return 0;
}
