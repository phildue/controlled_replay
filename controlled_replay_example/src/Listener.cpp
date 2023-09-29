#include "Listener.h"
using namespace std::chrono_literals;
namespace controlled_replay_example {

Listener::Listener(const rclcpp::NodeOptions& options) :
    rclcpp::Node("listener", options),
    _cliReady{create_client<controlled_replay_interfaces::srv::PlayNext>("/play_next")},
    _pub{create_publisher<std_msgs::msg::String>("/hearsay", 10)},
    _sub{create_subscription<std_msgs::msg::String>(
      "/chatter",
      10,
      [&](std_msgs::msg::String::ConstSharedPtr msg) {
        _ctr++;
        std::this_thread::sleep_for(get_parameter("task_time").as_double() * 1000ms);  // heavy task
        auto rq = std::make_shared<controlled_replay_interfaces::srv::PlayNext::Request>();
        rq->num_messages = 1;
        _cliReady->async_send_request(rq);
        _pub->publish(*msg);
      })},
    _timer{create_wall_timer(1s, [&]() { RCLCPP_INFO(get_logger(), "Received messages: %ld", _ctr); })} {
  declare_parameter("task_time", 1.0);
}

}  // namespace controlled_replay_example
#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(controlled_replay_example::Listener)