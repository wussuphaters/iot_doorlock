/*****************************************************************************
 * 
 * Header file defining all functions related to wireless LAN interface.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define WIFI_SSID "tuveuxdupoulet"
#define WIFI_PASSWORD "Mcwalt3r15bacK"

bool init_wifi() {
  int attempts = 0;
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi network...");
    while(WiFi.status() != WL_CONNECTED && attempts <= 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    if(WiFi.status() == WL_CONNECTED) {
      Serial.println(" Connected successfully");
      ArduinoOTA
        .onStart([]() {
          String type;
          if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
          else // U_SPIFFS
            type = "filesystem";
    
          // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
          Serial.println("Start updating " + type);
        })
        .onEnd([]() {
          Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
          Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
          Serial.printf("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
          else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
          else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
          else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
          else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });
      ArduinoOTA.setHostname("Serrure connectée");
      ArduinoOTA.setPasswordHash("f16b58c99b7071f60c8447a8e6e70237");
      ArduinoOTA.begin();
      return true;
    } else  {
      Serial.println("Failed to connect");
      return false;
    }
  } else return true;
}
