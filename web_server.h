#include <WebServer.h>

WebServer server(80);

void report_status()  {
  String state = unlocked ? "unlocked" : "locked";
  server.send(200, "text/json", String("{\"state\":\""+state+"\"}"));
}

void handle_control() {
  StaticJsonDocument<512> json_buffer;
  
  auto error = deserializeJson(json_buffer, server.arg(0));
  
  if(!error)  {
    if(json_buffer.containsKey("token") && json_buffer.containsKey("state")) {
      const char* token = json_buffer["token"];
      if(is_token_valid(String(token)))  {
        const char* state = json_buffer["state"];
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
