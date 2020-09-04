/*****************************************************************************
 * 
 * Header file defining all functions related to wireless LAN interface.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <WiFi.h>

#define WIFI_SSID "tuveuxdupoulet"
#define WIFI_PASSWORD "Mcwalt3r15bacK"

bool init_wifi() {
  int attempts = 0;
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi network...");
  while(WiFi.status() != WL_CONNECTED && attempts <= 30) {
      delay(500);
      Serial.print(".");
      attempts++;
  }
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println(" Connected successfully");
    return true;
  } else  {
    Serial.println("Failed to connect");
    return false;
  }
}
