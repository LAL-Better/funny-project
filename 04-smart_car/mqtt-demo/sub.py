import paho.mqtt.client as mqtt
 
class MqttRoad(object):
 
    def __init__(self, mqtt_host, mqtt_port, mqtt_keepalive):
        super(MqttRoad, self).__init__()
        client = mqtt.Client()
        client.on_connect = self.on_connect
        client.on_message = self.on_message
        client.on_publish = self.on_publish
        client.connect(mqtt_host, mqtt_port, mqtt_keepalive)  # 600为keepalive的时间间隔
        client.loop_forever()  # 保持连接
 
    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code: " + str(rc))
        # 订阅
        client.subscribe("mqtt11")
 
 
    def on_message(self, client, userdata, msg):
        print("on_message topic:" + msg.topic + " message:" + str(msg.payload.decode('utf-8')))
        
 
    #   订阅回调
    def on_subscribe(self, client, userdata, mid, granted_qos):
        print("On Subscribed: qos = %d" % granted_qos)
        pass
 
    #   取消订阅回调
    def on_unsubscribe(self, client, userdata, mid):
        # print("取消订阅")
        print("On unSubscribed: qos = %d" % mid)
        pass
 
    #   发布消息回调
    def on_publish(self, client, userdata, mid):
        # print("发布消息")
        print("On onPublish: qos = %d" % mid)
        pass
 
    #   断开链接回调
    def on_disconnect(self, client, userdata, rc):
        # print("断开链接")
        print("Unexpected disconnection rc = " + str(rc))
        pass
 
 
if __name__ == '__main__':
    
    MqttRoad("10.161.142.247", 1883, 600)
