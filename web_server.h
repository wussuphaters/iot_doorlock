#include <WebServer.h>

WebServer server(80);

void report_status()  {
  String state = unlocked ? "unlocked" : "locked";
  server.send(200, "text/json", String("{\"state\":\""+state+"\"}"));
}

void handle_control() {
  StaticJsonBuffer<600> json_buffer;
  JsonObject& json_msg = json_buffer.parseObject(server.arg(0));
  
  if(json_msg.success())  {
    if(json_msg.containsKey("token") && json_msg.containsKey("state")) {
      const char* token = json_msg["token"];
      if(is_token_valid(String(token)))  {
        const char* state = json_msg["state"];
        if(String(state) == "lock") close_lock();
        else if(String(state) == "unlock") open_lock();
        server.send(200, "text/json", "{\"message\":\"Successfully controlled doorlock\"}");
      } else  {
        server.send(401, "text/json", "{\"error\":\"Invalid JSON Web token\"}");
      }
    } else  {
      server.send(400, "text/json", "{\"error\":\"Missing JSON keys\"}");
    }
  } else  {
    server.send(400, "text/json", "{\"error\":\"Invalid JSON format\"}");
  }
}

void init_web_server()  {
  server.on("/status", HTTP_ANY, report_status);
  
  server.on("/control", HTTP_POST, handle_control);
  
  server.begin();
}
