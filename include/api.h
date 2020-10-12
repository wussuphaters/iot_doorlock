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
String password = "3bf6g%pXV8lAh6$K@5Vw";
String jwt = "";

String login()  {
  if(WiFi.status() == WL_CONNECTED) {
    api.begin(String(API_ADDR)+"device/login.php");
    api.addHeader("Content-Type", "application/json");
    api.addHeader("charset", "utf-8");
    int responseCode = api.POST("{\"ip\":\"" + WiFi.localIP().toString() + "\",\"password\":\"" + password + "\"}");
    if(responseCode < 0)  {
      Serial.println("Request error, trying again");
      return login();
    } else if(responseCode == 200) {
      Serial.println("API validated token");
      StaticJsonDocument<512> json_msg;
      
      auto error = deserializeJson(json_msg, api.getString());
      if(error) {
        Serial.println("JSON parsing error");
      } else  {
        Serial.println("Successfully parsed JSON message");
        if(json_msg.containsKey("token")) {
          return json_msg["token"];
        } else  {
          Serial.println("Error retrieving JWT");
        }
      }
    } else  {
      Serial.println("Login failed");
    }
  } else  {
    Serial.println("WiFi disconnected");
    init_wifi();
    return login();
  }
  return "";
 }

void log_activity(String msg, int user_id)  {
  if(WiFi.status() == WL_CONNECTED) {
    if(jwt != "") {
      api.begin(String(API_ADDR)+"device/log_activity.php");
      api.addHeader("Content-Type", "application/json");
  
      String json_str = "{\"token\":\"" + jwt + "\",\"log\":{\"msg\":\"" + msg + "\", \"user_id\":" + user_id + "}}";
      int responseCode = api.POST(json_str);
      if(responseCode < 0) log_activity(msg, user_id);
    } else  {
      jwt = login();
      log_activity(msg, user_id);
    }
  } else  {
    Serial.println("WiFi disconnected");
    if(init_wifi()) log_activity(msg, user_id);
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

DynamicJsonDocument get_users() {
  DynamicJsonDocument json_buffer(5000);
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Attempting to retrieve user pin list...");
  
    api.begin(String(API_ADDR)+"user/get_pins.php");
    int response_code = api.GET();
  
    if(response_code < 0) {
      Serial.println("Request error, trying again");
      return get_users();
    } else if(response_code == 200) {
      String response = api.getString();
  
      auto error = deserializeJson(json_buffer, response);
  
      if(!error)  {
        Serial.println("Successfully retrieved user pin list");
      } else  {
        Serial.println("Could not retrieve user pin list (JSON error)");
        json_buffer.createNestedObject("pins");
      }
    } else  {
      Serial.println("Could not retrieve user pin list, trying again");
      json_buffer.createNestedObject("pins");
    }
  } else  {
    Serial.println("WiFi not connected, could not retrieve user pin list");
    json_buffer.createNestedObject("pins");
  }

  return json_buffer;
}
