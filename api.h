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

 #define API_ADDR "192.168.1.50/api/users/"
 #define DEVICE_NAME "IOT_Doorlock"
 #define DEVICE_PASSWORD "qwerty"

 HTTPClient api;
 String api_access_token = "";

 bool login_api() {
  if(api_access_token != "") return true;
  else if(WiFi.status() == WL_CONNECTED) {
    String str_payload = "{\"name\":\""+String(DEVICE_NAME)+"\",\"password\":\""+String(DEVICE_PASSWORD)+"\"}";

    api.begin(String(API_ADDR)+"login.php");
    api.addHeader("Content-Type", "text/plain");
    int response_code = api.POST(str_payload);

    if(response_code == 200) {
      String response = api.getString();
      StaticJsonBuffer<200> json_buffer;

      JsonObject& json_response = json_buffer.parseObject(response);

      if(json_response.success() && json_response.containsKey("token"))  {
        json_response["token"].printTo(api_access_token);
        return true;
      } else  {
        return false;
      }
    } else  {
      return false;
    }
  } else  {
    return false;
  }
 }
 
 JsonObject& get_users() {
  StaticJsonBuffer<200> json_buffer;
  
  if(WiFi.status() == WL_CONNECTED && login_api()) {
    String str_payload = "{\"token\":\""+api_access_token+"\"}";

    api.begin(String(API_ADDR)+"get_all.php");
    api.addHeader("Content-Type", "text/plain");
    int response_code = api.POST(str_payload);

    if(response_code == 200) {
      String response = api.getString();

      JsonObject& json_response = json_buffer.parseObject(response);

      if(json_response.success())  {
        return json_response;
      } else  {
        return json_buffer.createObject();
      }
    } else  {
      return json_buffer.createObject();
    }
  } else  {
    return json_buffer.createObject();
  }
 }
