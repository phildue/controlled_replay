#include "Talker.h"
using namespace std::chrono_literals;

namespace controlled_replay_example {

Talker::Talker(const rclcpp::NodeOptions& options) :
    rclcpp::Node("Talker", options),
    _pub{create_publisher<std_msgs::msg::String>("/chatter", 10)},
    _timer{create_wall_timer(10ms, [&]() {
      _pub->publish(*std::make_shared<std_msgs::msg::String>());
      RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 1000, "Publishing...");
    })} {}

}  // namespace controlled_replay_example
#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(controlled_replay_example::Talker)