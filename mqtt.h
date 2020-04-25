#include <PubSubClient.h>
#include "wireless_lan.h"

#define MQTT_BROKER ""
#define MQTT_PORT 1883
#define MQTT_PASSWORD ""
#define MQTT_RECONNECT_TIMEOUT 5000

#define DELETED_USERS_TOPIC "deleted_users"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

unsigned long last_mqtt_connect_attempt = 0;

void on_message(char* topic, byte* message, unsigned int length)  {
  String msg_str;
  
  for (int i = 0; i < length; i++) {
    msg_str += (char)message[i];
  }

  if(String(topic) == DELETED_USERS_TOPIC)  {
    //delete_fingerprint(int(msg_str));
  }
}

void init_mqtt()  {
  mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt_client.setCallback(on_message);
  
  Serial.print("Connecting to MQTT broker..");
  if (mqtt_client.connect("IOT Doorlock")) {
    Serial.println("Connected");
    mqtt_client.subscribe(DELETED_USERS_TOPIC);
  } else {
    Serial.print("Failed, rc=");
    Serial.print(mqtt_client.state());
    last_mqtt_connect_attempt = millis();
  }
}

void reconnect_mqtt() {
  while (!mqtt_client.connected() && (millis() > (last_mqtt_connect_attempt + MQTT_RECONNECT_TIMEOUT) | last_mqtt_connect_attempt > millis())) {
    mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt_client.setCallback(on_message);
    
    Serial.print("MQTT broker disconnected, reconnecting...");
    if (mqtt_client.connect("IOT Doorlock")) {
      Serial.println("Connected");
      mqtt_client.subscribe(DELETED_USERS_TOPIC);
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(mqtt_client.state());
      last_mqtt_connect_attempt = millis();
    }
  }
}
