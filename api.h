/*****************************************************************************
 * 
 * Header file defining all functions related to the projects' web API.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

 #include <HTTPClient.h>
 #include <ArduinoJson.h>
 #include "wireless_lan.h"

 #define API_ADDR "http://192.168.1.100/smart_home_api/api/"

 HTTPClient api;

 bool is_token_valid(String token)  {
  if(WiFi.status() == WL_CONNECTED) {
    api.begin(String(API_ADDR)+"user/validate_token.php");
    api.addHeader("Content-Type", "application/json");
    
    int responseCode = api.POST("{\"token\":\""+token+"\"}");

    if(responseCode < 0)  {
      Serial.println("Request error, trying again");
      return is_token_valid(token);
    } else if(responseCode == 200) {
      Serial.println("API validated token");
      return true;
    } else  {
      Serial.println("API did not validat token");
      return false;
    }
  } else  {
    Serial.println("WiFi disconnected");
    if(init_wifi()) return is_token_valid(token);
    else return false;
  }
 }
 
StaticJsonDocument<5000> get_users() {
  StaticJsonDocument<5000> json_buffer;
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Attempting to retrieve user pin list...");

    api.begin(String(API_ADDR)+"user/get_pins.php");
    int response_code = api.GET();

    if(response_code == 200) {
      String response = api.getString();

      auto error = deserializeJson(json_buffer, response);

      if(!error)  {
        Serial.println("Successfully retrieved user pin list");
        return json_buffer;
      } else  {
        Serial.println("Could not retrieve user pin list");
        return json_buffer;
      }
    } else  {
      Serial.println("Could not retrieve user pin list");
      return json_buffer;
    }
  } else  {
    Serial.println("WiFi not connected, could not retrieve user pin list");
    return json_buffer;
  }
 }
