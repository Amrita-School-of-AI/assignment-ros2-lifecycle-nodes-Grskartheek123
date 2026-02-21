#include <chrono>
#include <memory>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;
using rclcpp_lifecycle::LifecycleNode;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class LifecycleSensor : public LifecycleNode
{
public:
  LifecycleSensor()
  : LifecycleNode("lifecycle_sensor"),
    rng_(std::random_device{}()),
    dist_(0.0, 100.0)
  {
    RCLCPP_INFO(get_logger(), "LifecycleSensor created (unconfigured).");
  }

protected:
  CallbackReturn on_configure(const rclcpp_lifecycle::State &)
  {
    publisher_ = this->create_publisher<std_msgs::msg::Float64>("/sensor_data", 10);
    RCLCPP_INFO(get_logger(), "Sensor configured");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &)
  {
    // Start timer only when active
    timer_ = this->create_wall_timer(500ms, std::bind(&LifecycleSensor::publish_sensor, this));
    RCLCPP_INFO(get_logger(), "Sensor activated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &)
  {
    // Stop timer
    timer_.reset();
    RCLCPP_INFO(get_logger(), "Sensor deactivated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &)
  {
    // Reset publisher and timer
    timer_.reset();
    publisher_.reset();
    RCLCPP_INFO(get_logger(), "Sensor cleaned up");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &)
  {
    RCLCPP_INFO(get_logger(), "Sensor shutting down");
    return CallbackReturn::SUCCESS;
  }

private:
  void publish_sensor()
  {
    if (!publisher_) return;

    std_msgs::msg::Float64 msg;
    msg.data = dist_(rng_);

    publisher_->publish(msg);

    RCLCPP_INFO(get_logger(), "Publishing sensor data: %.2f", msg.data);
  }

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;

  std::mt19937 rng_;
  std::uniform_real_distribution<double> dist_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<LifecycleSensor>();

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node->get_node_base_interface());
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
