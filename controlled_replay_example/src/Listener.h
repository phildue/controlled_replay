
#pragma once
#include <controlled_replay_interfaces/srv/play_next.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include "controlled_replay_example/visibility_control.h"
namespace controlled_replay_example {
class Listener : public rclcpp::Node {
public:
  COMPOSITION_PUBLIC

  Listener(const rclcpp::NodeOptions& options);

private:
  rclcpp::Client<controlled_replay_interfaces::srv::PlayNext>::SharedPtr _cliReady;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr _pub;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr _sub;

  rclcpp::TimerBase::SharedPtr _timer;
  std::int64_t _ctr;
};
}  // namespace controlled_replay_example