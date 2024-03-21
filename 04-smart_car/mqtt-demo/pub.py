# -*- coding: utf-8 -*-# -*- coding: utf-8 -*-
 
 
import paho.mqtt.client as mqtt
import time
 
 
def on_connect(client, userdata, flags, rc):
    print("链接")
    print("Connected with result code: " + str(rc))
 
 
def on_message(client, userdata, msg):
    print("消息内容")
    print(msg.topic + " " + str(msg.payload))
 
 
#   订阅回调
def on_subscribe(client, userdata, mid, granted_qos):
    print("订阅")
    print("On Subscribed: qos = %d" % granted_qos)
    pass
 
 
#   取消订阅回调
def on_unsubscribe(client, userdata, mid, granted_qos):
    print("取消订阅")
    print("On unSubscribed: qos = %d" % granted_qos)
    pass
 
 
#   发布消息回调
def on_publish(client, userdata, mid):
    print("发布消息")
    print("On onPublish: qos = %d" % mid)
    pass
 
 
#   断开链接回调
def on_disconnect(client, userdata, rc):
    print("断开链接")
    print("Unexpected disconnection rc = " + str(rc))
    pass
 
 
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.on_publish = on_publish
client.on_disconnect = on_disconnect
client.on_unsubscribe = on_unsubscribe
client.on_subscribe = on_subscribe
client.connect('10.161.142.247', 1883, 600) # 600为keepalive的时间间隔
while True:
    client.publish(topic='mqtt11', payload='amazing123', qos=0, retain=False)
    time.sleep(2)
