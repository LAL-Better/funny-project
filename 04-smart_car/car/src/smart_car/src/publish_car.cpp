#include <chrono>
#include <memory>
extern "C" {
#include "smart_car/smart_car.h"
}

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
  }
  smart_car_ret publish_msg(char* msg)
  {
    auto message = smart_car::msg::Car();
    message.direction = message.D_UP;
    message.note = "this is for test! ";
    if(msg)
      message.note = msg;
    RCLCPP_INFO(this->get_logger(), "Direction: '%d' note: '%s'", message.direction, message.note.c_str());
    this->smart_car_publisher_->publish(message);
    return SMART_CAR_OK;
  }

private:
  //变量声明
  rclcpp::Publisher<smart_car::msg::Car>::SharedPtr smart_car_publisher_;
  //rclcpp::TimerBase::SharedPtr timer_;
};

smart_car_ret msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
  
    if(!context){
      printf("context is null!");
      return SMART_CAR_MSG_NULL;
    }

    //https://stackoverflow.com/questions/10072004/c-typecast-cast-a-pointer-from-void-pointer-to-class-pointer
    CarPublisher* pub_msg = static_cast<CarPublisher *>(context);
    char *mqtt_msg_res = reinterpret_cast<char *>(message->payload);
    std::cout<<"Message arrived"<<std::endl;
    std::cout<<"    topic: "<<topicName<<"in "<< topicLen<<std::endl;
    std::cout<<"    message: "<< mqtt_msg_res<<std::endl;

    pub_msg->publish_msg(mqtt_msg_res);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return SMART_CAR_MQTT_OK;
}

int main(int argc, char * argv[])
{
  char* mqtt_topic;
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CarPublisher>();

  mqtt_client* MqttClient;
  
  if(mqtt_client_init(MQTT_DIR_SUB, NULL, NULL, &MqttClient) != SMART_CAR_OK){
    return SMART_CAR_MQTT_INIT_FAIL;
  }
  MqttClient->msgarrvd = msgarrvd;

  MqttClient->mqtt_client_open(MqttClient, node.get());

  mqtt_topic = (char*)malloc(8*sizeof(char));
  strcpy(mqtt_topic, TOPIC);
  MqttClient->mqtt_client_subscribe(mqtt_topic, QOS, MqttClient);
  free(mqtt_topic);
  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
