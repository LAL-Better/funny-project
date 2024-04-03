#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <MQTTAsync.h>
#include <MQTTClient.h>


#define ADDRESS     "tcp://10.161.142.247:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "mqtt11"
#define PAYLOAD     "Hello World!"
#define QOS         1


typedef enum{
    SMART_CAR_OK = 0,
    SMART_CAR_MQTT_OK =1,
    SMART_CAR_ERR_UNKNOW =2,
    SMART_CAR_ERR_DIR =3,
} smart_car_ret;

typedef enum{
    MQTT_DIR_UNKNOW = 0,
    MQTT_DIR_PUB =1,
    MQTT_DIR_SUB =2,
    MQTT_DIR_DUL =3,
} mqtt_op_dir;

typedef struct mqtt_client_s {
    char*   address;
    char*   clientid;
    char*   topic;
    char*   payload;
    int     qos;
    int     timeout;

    MQTTClient client;
    volatile MQTTClient_deliveryToken deliveredtoken;

    smart_car_ret   (*delivered)(void *context, MQTTClient_deliveryToken dt, volatile MQTTClient_deliveryToken *deliveredtoken);
    smart_car_ret   (*msgarrvd)(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    smart_car_ret   (*connlost)(void *context, char *cause);
    smart_car_ret   (*mqtt_client_publish)(char *top, int qos, char *msg, int len, struct mqtt_client_s *client_s);
    smart_car_ret   (*mqtt_client_subscribe)(char *top, int qos, struct mqtt_client_s *client_s);
    smart_car_ret   (*mqtt_client_open)(struct mqtt_client_s *client_s);
}mqtt_client;

smart_car_ret mqtt_client_init(mqtt_op_dir dir, char* clientid, char* address, int qos, mqtt_client *client_s);

/*use class to achieve mqtt client
class MqttClient
{
    public:
    MqttClient()
    {

    }
    smart_car_ret delivered(void *context, MQTTClient_deliveryToken dt);
    smart_car_ret msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);
    private:
    static MQTTClient client = NULL;
    volatile MQTTClient_deliveryToken deliveredtoken;
    char* address;
    char* clientid;
    char*
};
*/