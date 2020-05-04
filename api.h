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

 HTTPClient api;
 
 JsonObject& get_users() {
  StaticJsonBuffer<5000> json_buffer;
  
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("Attempting to retrieve user pin list...");

    api.begin(String(API_ADDR)+"user/get_pins.php");
    int response_code = api.GET();

    if(response_code == 200) {
      String response = api.getString();

      JsonObject& json_response = json_buffer.parseObject(response);

      if(json_response.success())  {
        Serial.println("Successfully retrieved user pin list");
        return json_response;
      } else  {
        Serial.println("Could not retrieve user pin list");
        return json_buffer.createObject();
      }
    } else  {
      Serial.println("Could not retrieve user pin list");
      return json_buffer.createObject();
    }
  } else  {
    Serial.println("WiFi not connected, could not retrieve user pin list");
    return json_buffer.createObject();
  }
 }
