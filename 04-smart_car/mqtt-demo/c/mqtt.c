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
#define TIMEOUT     10000L

static MQTTClient client = NULL;
volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
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

    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int mqtt_client_publish(char *top, int qos, char *msg, int len)
{
    MQTTClient_deliveryToken token;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    if (client == NULL)
        return -1;

    pubmsg.payload = msg;
    pubmsg.payloadlen = len;
    pubmsg.qos = qos;
    pubmsg.retained = 0;
    deliveredtoken = 0;
    MQTTClient_publishMessage(client, top, &pubmsg, &token);
    printf("Waiting for publication of %s\n"
            "on topic %s for client\n",
            msg, top);
    while (deliveredtoken != token);
    return 0;
}

int mqtt_client_subscribe(char *top, int qos)
{
    if (client == NULL)
	return -1;

    MQTTClient_subscribe(client, top, qos);
    return 0;
}

int mqtt_client_open()
{
    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
   
    MQTTClient_create(&client, ADDRESS, CLIENTID,        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        return -1;
    }
    printf("Success to connect!\n");
    return 0;
}

int main(int argc, char* argv[])
{
    char *str = "hello world!";

    mqtt_client_open();
    mqtt_client_subscribe(TOPIC, QOS);
    mqtt_client_publish(TOPIC, QOS, str, strlen(str));
    sleep(1);

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    
    return 0;
}
