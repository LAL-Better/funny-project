#include "smart_car/smart_car.h"



static smart_car_ret delivered(void *context, MQTTClient_deliveryToken dt, volatile MQTTClient_deliveryToken *deliveredtoken)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
    return SMART_CAR_OK;
}

static smart_car_ret msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");

    payloadptr = message->payload;
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return SMART_CAR_OK;
}

static smart_car_ret connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
    
    return SMART_CAR_OK;
}

static smart_car_ret mqtt_client_publish(char *top, int qos, char *msg, int len, mqtt_client *client_s)
{
    MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    if (client_s->client == NULL)
        return -1;

    pubmsg.payload = msg;
    pubmsg.payloadlen = len;
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    client_s->deliveredtoken = 0;
    MQTTClient_publishMessage(client_s->client, top, &pubmsg, &token);
    printf("Waiting for publication of %s\n"
            "on topic %s for client\n",
            msg, top);
    while (client_s->deliveredtoken != token);
    return SMART_CAR_OK;
}

/// @brief 
/// @param top 
/// @param qos 
/// @param client_s 
/// @return 
static smart_car_ret mqtt_client_subscribe(char *top, int qos, mqtt_client *client_s)
{
    if (client_s->client == NULL)
	return -1;

    MQTTClient_subscribe(client_s->client, top, qos);
    return SMART_CAR_OK;
}

static smart_car_ret mqtt_client_open(mqtt_client *client_s)
{
    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
   
    MQTTClient_create(&(client_s->client), client_s->address, client_s->clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client_s->client, NULL, client_s->connlost, client_s->msgarrvd, client_s->delivered);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client_s->client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        return SMART_CAR_ERR_UNKNOW;
    }
    printf("Success to connect!\n");
    return SMART_CAR_OK;
}

/*
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
*/

smart_car_ret mqtt_client_init(mqtt_op_dir dir, char* clientid, char* address, int qos, mqtt_client *client_s)
{
    if(dir == 0){
        printf("dir or mqtt client can't be null\n");
        return SMART_CAR_ERR_DIR;
    }

    if(!client_s){
        client_s = malloc(sizeof(*client_s));
    }
    client_s->delivered = delivered;
    client_s->msgarrvd = msgarrvd;
    client_s->connlost = connlost;
    client_s->mqtt_client_publish = mqtt_client_publish;
    client_s->mqtt_client_subscribe = mqtt_client_subscribe;
    client_s->mqtt_client_open = mqtt_client_open;

    client_s->address = !address?address:ADDRESS;
    client_s->clientid = !clientid?clientid:CLIENTID;


    mqtt_client_open();
    mqtt_client_subscribe(TOPIC, QOS);
    mqtt_client_publish(TOPIC, QOS, str, strlen(str));
    while(1);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    
    return SMART_CAR_OK;
}