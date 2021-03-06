#include <WebServer.h>

WebServer server(80);

void delete_fingerprints()  {
  StaticJsonDocument<512> json_buffer;
  
  auto error = deserializeJson(json_buffer, server.arg(0));
  
  if(!error)  {
    if(json_buffer.containsKey("token")) {
      const char* token = json_buffer["token"];
      int user_id = is_token_valid(String(token));
      if(user_id >= 0)  {
        fpScanner.deleteModel(String(user_id).toInt());
        server.send(200, "text/json", "{\"message\":\"Successfully deleted user #" + String(user_id) + " fingerprint from sensor database\"}");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        log_activity("User #" + String(user_id) + " fingerprint deleted from sensor database from web server", user_id);
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

void enroll_fingerprint() {
  StaticJsonDocument<512> json_buffer;
  
  auto error = deserializeJson(json_buffer, server.arg(0));
  
  if(!error)  {
    if(json_buffer.containsKey("token")) {
      const char* token = json_buffer["token"];
      int user_id = is_token_valid(String(token));
      if(user_id >= 0)  {
        if(unlocked)  {
          server.send(200, "text/json", "{\"message\":\"Enrolling new fingerprint for user #" + String(user_id) + "\"}");
          fpScanner.deleteModel(String(user_id).toInt());
          if(add_fingerprint(user_id))  {
            Serial.println("Fingerprint successfully enrolled");
            display_unlock();
            beep_ok();
            log_activity("New fingerprint successfully enrolled", user_id);
          } else  {
            Serial.println("Fingerprint enroll failed");
            display_error();
            beep_nok();
            log_activity("New fingerprint enroll failed attempt", user_id);
          }
        } else server.send(403, "text/json", "{\"message\":\"Door must be unlocked to enroll new fingerprint\"}");
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
      int user_id = is_token_valid(String(token));
      if(user_id >= 0)  {
        const char* state = json_buffer["state"];
        if(String(state) == "lock") close_lock();
        else if(String(state) == "unlock") open_lock();
        server.send(200, "text/json", "{\"message\":\"Successfully controlled doorlock\"}");
        log_activity((unlocked ? "Door unlocked" : "Door locked")+String(" from web server"), user_id);
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
  server.on("/delete_fp", HTTP_POST, delete_fingerprints);
  server.on("/enroll_fp", HTTP_POST, enroll_fingerprint);
  
  server.begin();
}
