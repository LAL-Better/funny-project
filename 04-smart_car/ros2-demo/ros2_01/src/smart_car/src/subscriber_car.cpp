#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "smart_car/msg/car.hpp"//包含信息列表的头文件, 规则举例Car.msg->car.h；CarAbc.msg->car_abc.h

using namespace std::chrono_literals;
//以下创建一个节点及一个 smart_car TOPIC的监听者
class CarSubscriber : public rclcpp::Node
{
public:
  CarSubscriber()
  : Node("smart_car_subscriber")
  {
    smart_car_subscriber_ =
      this->create_subscription<smart_car::msg::Car>("smart_car", 
      100, std::bind(&CarSubscriber::topic_callback, this, std::placeholders::_1));
  }

private:
//以下创建一个消息来到时的调用函数
  void topic_callback(const smart_car::msg::Car::SharedPtr msg) const
  {
    RCLCPP_INFO(this->get_logger(), "Direction: '%d' note: '%s'", msg->direction, msg->note.c_str());
  }
  rclcpp::Subscription<smart_car::msg::Car>::SharedPtr smart_car_subscriber_;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CarSubscriber>());
  rclcpp::shutdown();

  return 0;
}