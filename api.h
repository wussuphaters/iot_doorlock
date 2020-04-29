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

 #define API_ADDR "http://192.168.1.100/smart_home_api/api/"
 #define DEVICE_NAME "IOT_Doorlock"
 #define DEVICE_PASSWORD "qwerty"

 HTTPClient api;
 String api_access_token = "";

 bool login_api() {
  if(api_access_token != "") return true;
  else if(WiFi.status() == WL_CONNECTED) {
    Serial.print("Authenticating to API...");
    
    String str_payload = "{\"name\":\""+String(DEVICE_NAME)+"\",\"password\":\""+String(DEVICE_PASSWORD)+"\"}";

    api.begin(String(API_ADDR)+"host/login.php");
    api.addHeader("Content-Type", "text/plain");
    int response_code = api.POST(str_payload);

    if(response_code == 200) {
      String response = api.getString();
      
      StaticJsonBuffer<1000> json_buffer;

      JsonObject& json_response = json_buffer.parseObject(response);
      if(json_response.success())  {
        if(json_response.containsKey("token"))  {
          Serial.println("Successfully retrieved JWT");
          json_response["token"].printTo(api_access_token);
          return true;
        } else  {
          Serial.println("Failed to retrieve JWT");
          return false;
        }
      } else  {
        Serial.println("Failed to retrieve JWT");
        return false;
      }
    } else  {
      Serial.println("Login failed");
      return false;
    }
  } else  {
    Serial.println("Login failed");
    return false;
  }
 }
 
 JsonObject& get_users() {
  StaticJsonBuffer<5000> json_buffer;
  
  if(WiFi.status() == WL_CONNECTED && login_api()) {
    Serial.println("Attempting to retrieve user list...");
    String str_payload = "{\"token\":"+api_access_token+"}";

    api.begin(String(API_ADDR)+"user/get_all.php");
    api.addHeader("Content-Type", "text/plain");
    int response_code = api.POST(str_payload);

    if(response_code == 200) {
      String response = api.getString();

      JsonObject& json_response = json_buffer.parseObject(response);

      if(json_response.success())  {
        Serial.println("Successfully retrieved user list");
        return json_response;
      } else  {
        Serial.println("Could not retrieve user list");
        return json_buffer.createObject();
      }
    } else  {
      Serial.println("Could not retrieve user list");
      return json_buffer.createObject();
    }
  } else  {
    Serial.println("WiFi not connected, could not retrieve user list");
    return json_buffer.createObject();
  }
 }
