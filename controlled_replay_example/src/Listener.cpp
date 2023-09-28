#include "Listener.h"
using namespace std::chrono_literals;
namespace controlled_replay_example {

Listener::Listener(const rclcpp::NodeOptions& options) :
    rclcpp::Node("Listener", options),
    _cliReady{create_client<controlled_replay_interfaces::srv::Ready>("/ready")},
    _sub{create_subscription<std_msgs::msg::String>(
      "/chatter",
      10,
      [&](std_msgs::msg::String::ConstSharedPtr msg) {
        _ctr++;
        std::this_thread::sleep_for(1s);  // heavy task
        auto rq = std::make_shared<controlled_replay_interfaces::srv::Ready::Request>();
        rq->isready = true;
        _cliReady->async_send_request(rq);
      })},
    _timer{create_wall_timer(1s, [&]() { RCLCPP_INFO(get_logger(), "Received messages: %ld", _ctr); })} {}

}  // namespace controlled_replay
#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(controlled_replay_example::Listener)