#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "smart_car/msg/car.hpp" //包含信息列表的头文件, 规则举例Car.msg->car.h；CarAbc.msg->car_abc.h

using namespace std::chrono_literals;
//以下创建一个节点及一个 smart_car TOPIC的发布者
class CarPublisher : public rclcpp::Node
{
public:
  CarPublisher()
  : Node("smart_car_publisher")
  {
    smart_car_publisher_ =
      this->create_publisher<smart_car::msg::Car>("smart_car", 100);

//以下创建一个定时回调函数
    auto publish_msg = [this]() -> void {
        auto message = smart_car::msg::Car();

        message.direction = message.D_DOWN;
        message.note = "this is for test 2 ! ";

        RCLCPP_INFO(this->get_logger(), "Direction: '%d' note: '%s'", message.direction, message.note.c_str());

        this->smart_car_publisher_->publish(message);
      };
    //创建一个定时器，使每秒调用一次publish_msg函数
    timer_ = this->create_wall_timer(1s, publish_msg);
  }

private:
  //变量声明
  rclcpp::Publisher<smart_car::msg::Car>::SharedPtr smart_car_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CarPublisher>());
  rclcpp::shutdown();

  return 0;
}