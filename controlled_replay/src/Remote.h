
#pragma once
#include "rclcpp/rclcpp.hpp"
#include <rosbag2_interfaces/srv/burst.hpp>
#include <rosbag2_interfaces/srv/resume.hpp>

#include "controlled_replay/visibility_control.h"
#include <controlled_replay_interfaces/srv/play_next.hpp>
namespace controlled_replay {
class Remote : public rclcpp::Node {
public:
  COMPOSITION_PUBLIC

  Remote(const rclcpp::NodeOptions& options);

private:
  rclcpp::Service<controlled_replay_interfaces::srv::PlayNext>::SharedPtr _servicePlay;
  std::map<std::string, bool> _ready;
  rclcpp::Client<rosbag2_interfaces::srv::Resume>::SharedPtr _cliResume;
  rclcpp::Client<rosbag2_interfaces::srv::Burst>::SharedPtr _cliBurst;
  const int _nBurst;
  rclcpp::Time _t0;
  rclcpp::TimerBase::SharedPtr _timer, _timerStop;
  int _tWaiting;
  void requestNextBatch();
  void shutdownWhenEnded();
};
}  // namespace controlled_replay