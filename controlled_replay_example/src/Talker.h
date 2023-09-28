
#pragma once
#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/string.hpp>

#include "controlled_replay_example/visibility_control.h"
namespace controlled_replay_example {
class Talker : public rclcpp::Node {
public:
  COMPOSITION_PUBLIC

  Talker(const rclcpp::NodeOptions& options);

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr _pub;
  rclcpp::TimerBase::SharedPtr _timer;
};
}  // namespace controlled_replay_example