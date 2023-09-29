#include "Remote.h"

using namespace std::chrono_literals;
// TODO should be use sim time by default, shutdown off by default
namespace controlled_replay {
Remote::Remote(const rclcpp::NodeOptions& options) :
    rclcpp::Node("remote", options),
    _servicePlay{create_service<controlled_replay_interfaces::srv::Ready>(
      "togglePlay",
      [this](
        std::shared_ptr<controlled_replay_interfaces::srv::Ready::Request> request,
        std::shared_ptr<controlled_replay_interfaces::srv::Ready::Response> response) {
        _ready[request->requester] = request->isready;
        response->isplaying = std::all_of(_ready.begin(), _ready.end(), [](auto pr) { return pr.second; });
      })},
    _cliResume{create_client<rosbag2_interfaces::srv::Resume>("/rosbag2_player/resume")},
    _cliBurst{create_client<rosbag2_interfaces::srv::Burst>("/rosbag2_player/burst")},
    _nBurst{static_cast<int>(declare_parameter("batch_size", 10))},
    _t0{get_clock()->now()},
    _timer{create_wall_timer(declare_parameter("period", 1.0) * 1000ms, std::bind(&Remote::requestNextBatch, this))},
    _timerStop{} {
  declare_parameter("automatic_shutdown", 5);
  if (!get_parameter("use_sim_time").as_bool()) {
    RCLCPP_WARN(get_logger(), "Use sim time is not enabled. Cannot determine when bag has ended..");
  }
}

void Remote::requestNextBatch() {
  if ((_ready.empty() || std::all_of(_ready.begin(), _ready.end(), [](auto pr) { return pr.second; }))) {
    auto rq = std::make_shared<rosbag2_interfaces::srv::Burst::Request>();
    rq->num_messages = _nBurst;
    _cliBurst->async_send_request(rq);
    for (auto& [id, pr] : _ready) {
      pr = false;
    }
  }
  if (!_timerStop && get_parameter("use_sim_time").as_bool()) {
    _timerStop = create_wall_timer(5s, std::bind(&Remote::shutdownWhenEnded, this));
  }
}

void Remote::shutdownWhenEnded() {
  if (get_clock()->now() <= _t0 && get_parameter("automatic_shutdown").as_int() > 0) {
    _cliResume->async_send_request(std::make_shared<rosbag2_interfaces::srv::Resume::Request>());
    _tWaiting = get_parameter("automatic_shutdown").as_int();
    _timerStop.reset();
    _timer = create_wall_timer(std::chrono::seconds(1), [&]() {
      RCLCPP_INFO(get_logger(), "Replay has ended. Will shutdown in %d seconds..", _tWaiting);
      _tWaiting--;
      if (_tWaiting <= 0) {
        rclcpp::shutdown();
      }
    });
  }
  _t0 = get_clock()->now();
}
}  // namespace controlled_replay
#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(controlled_replay::Remote)