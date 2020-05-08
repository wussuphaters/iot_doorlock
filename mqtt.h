/*****************************************************************************
 * 
 * Header file defining all functions related to the MQTT broker connection.
 * 
 * Written by Maël PONCHANT
 * 
 * mosquitto_sub -h localhost -t "hosts/doorlock/entries" -u "host" -P "cAE99wC@WwQh"
 * 
 * ***************************************************************************
 */

#include <PubSubClient.h>
#include "wireless_lan.h"

#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_USERNAME "client"
#define MQTT_PASSWORD "cAE99wC@WwQh"
#define MQTT_RECONNECT_TIMEOUT 10000
#define MQTT_STATUS_REPORT_TIMEOUT 3000

//Topics
#define DOORLOCK_ENTRIES_TOPIC "devices/doorlock/entries"
#define DOORLOCK_COMMANDS_TOPIC "devices/doorlock/commands"
#define DOORLOCK_STATUS_TOPIC "devices/doorlock/status"

//Commands
#define CMD_REBOOT "reboot"
#define CMD_DELETE_USER "delete_user"
#define CMD_UNLOCK "unlock"
#define CMD_LOCK "lock"

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

unsigned long last_mqtt_connect_attempt = 0;
unsigned long last_status_report = 0;

void report_status()  {
  if(mqtt_client.connected() && (millis() > (last_status_report + MQTT_STATUS_REPORT_TIMEOUT))) {
    Serial.println("Reporting doorlock status");
    String msg = unlocked ? "unlocked" : "locked";
    mqtt_client.publish(DOORLOCK_STATUS_TOPIC, String("{\"state\":\""+msg+"\"}").c_str());
    last_status_report = millis();
  }
}

void on_message(char* topic, byte* message, unsigned int length)  {
  String msg_str;
  
  for (int i = 0; i < length; i++) {
    msg_str += (char)message[i];
  }
  if(String(topic) == DOORLOCK_COMMANDS_TOPIC)  {
    Serial.println("Command received, processing");
    StaticJsonBuffer<200> json_buffer;
    JsonObject& json_msg = json_buffer.parseObject(msg_str);

    if(json_msg.success())  {
      if(json_msg.containsKey("command")) {
        const char* command = json_msg["command"];
        if(String(command) == CMD_REBOOT) {
          Serial.println("Rebooting");
          ESP.restart();
        } else if(String(command) == CMD_DELETE_USER) {
          delete_fingerprint(json_msg["arguments"]["user_id"]);
        } else if(String(command) == CMD_UNLOCK)  {
          open_lock();
        } else if(String(command) == CMD_LOCK)  {
          close_lock();
        }
      }
    }
  }
}

void init_mqtt()  {
  mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt_client.setCallback(on_message);
  
  Serial.print("Connecting to MQTT broker..");
  if (mqtt_client.connect(MQTT_USERNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
    Serial.println("Connected");
    mqtt_client.subscribe(DOORLOCK_COMMANDS_TOPIC);
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
    if (mqtt_client.connect(MQTT_USERNAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("Connected");
      mqtt_client.subscribe(DOORLOCK_COMMANDS_TOPIC);
    } else {
      Serial.print("Failed to connect, rc=");
      Serial.print(mqtt_client.state());
      last_mqtt_connect_attempt = millis();
    }
  }
}

void notify(int user_id) {
  if(mqtt_client.connected()) {
    mqtt_client.publish(DOORLOCK_ENTRIES_TOPIC, String(user_id).c_str());
  }
}
