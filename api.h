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

void log_activity(bool action, String action_method, int user_id)  {
  if(WiFi.status() == WL_CONNECTED) {
      api.begin(String(API_ADDR)+"user/log_activity.php");
      api.addHeader("Content-Type", "application/json");
  
      String json_str = "{\"log\":{\"device_ip\":\"" + String(WiFi.localIP()) +"\",\"state\":\"" + (action ? "unlocked" : "locked") + "\",\"method\":\"" + action_method + "\", \"user_id\":" + user_id + "}}";
      
      int responseCode = api.POST(json_str);
  } else  {
    Serial.println("WiFi disconnected");
    if(init_wifi()) log_activity(action, action_method, user_id);
    else return;
  }
}

void breach_alert()  {

}

int is_token_valid(String token)  {
  StaticJsonDocument<600> json_response;
  
  if(WiFi.status() == WL_CONNECTED) {
    api.begin(String(API_ADDR)+"user/validate_token.php");
    api.addHeader("Content-Type", "application/json");
    
    int responseCode = api.POST("{\"token\":\""+token+"\"}");
  
    if(responseCode < 0)  {
      Serial.println("Request error, trying again");
      return is_token_valid(token);
    } else if(responseCode == 200) {
      Serial.println("API validated token");
      String response = api.getString();
      
      auto error = deserializeJson(json_response, response);
  
      if(!error)  {
        Serial.println("Successfully parsed JSON response from API");
        if(json_response.containsKey("user_id")) {
          Serial.println("Successfully retrieved user id from API JSON response");
          return json_response["user_id"].as<int>();
        } else  {
          Serial.println("Could not retrieve user id from API JSON response");
          return 0;
        }
      } else  {
        Serial.println("Could not deserialize JSON response from API");
        return 0;
      }
    } else  {
      Serial.println("API did not validate token");
      return -1;
    }
  } else  {
    Serial.println("WiFi disconnected");
    if(init_wifi()) return is_token_valid(token);
    else return -1;
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
