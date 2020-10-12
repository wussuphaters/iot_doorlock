/*****************************************************************************
 * 
 * Main file for the IOT doorlock processing.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include "hardware.h"
#include "fingerprint_scanner.h"
#include "numpad.h"
#include "web_server.h"

#define PASSWD_TIMEOUT 5000
#define DOOR_CLOSING_TIME 2000

void setup() {
  Serial.begin(115200);
  init_display();
  display_text("Booting up");
  init_wifi();
  init_web_server();
  init_hardware();
  init_fingerprint_scanner();

  display_place_finger();
}

void loop() {
  server.handleClient();
  char key=numpad.getKey();

  if(key) {
    beep_keypress();
    if(key=='#' && unlocked)  {
        close_lock();
        log_activity(false, "outside", 0);
    }
    else if(!unlocked && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9' || key=='A' || key=='B' || key=='C' || key=='D'))  {
      String enteredPin="";
      int passwd_elapsed_time = millis();

      while(key != '*') {
        if(key=='#' && !unlocked) {
          enteredPin = "";
          display_place_finger();
        }
        else if(key && enteredPin.length() < 8) {
          enteredPin+=key;
          display_pin(enteredPin.length());
        }
  
        key=numpad.getKey();
        if(key) {
          beep_keypress();
          passwd_elapsed_time = millis();
        }
        if(millis() - passwd_elapsed_time >= PASSWD_TIMEOUT)  {
          Serial.println("Password timeout, aborting");
          break;
        }
      }
      if(enteredPin.length() > 0) {
        display_text("Wait...");
        int user_id = checkPin(enteredPin);
        if(user_id > 1)  {
          Serial.println("Access granted to user #"+String(user_id));
          if(!is_fingerprint_registered(user_id))  {
            Serial.println("User has no registered fingerprint, registering");
            int tries = 0;
            bool res = false;
            while(!res && tries < 3)  {
              res = add_fingerprint(user_id);
              tries++;
              Serial.println("Try "+String(tries)+" to register fingerprint failed");
              if(res) fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_BLUE, 3);
              else fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
            }
          }
          
          open_lock();
          log_activity(true, "from outside with personnal secret code", user_id);
        } else if(user_id == 1) {
          Serial.println("Master password used, access granted");
          open_lock();
          log_activity(true, "from outside with offline secret code", 0);
        } else if(user_id == 0)  {
          Serial.println("No pin match found, access denied");
          display_error();
          beep_nok();
          display_place_finger();
        } else  {
          Serial.println("User list could not be retrieved, master password must be used");
          display_text("Please use\nmaster pin");
          beep_nok();
          display_place_finger();
        }
        enteredPin="";
      }
    }
  }
  else if(digitalRead(PUSH_BUTTON)==HIGH) {
    on_button_press();
    log_activity(unlocked, "inside", 0);
  }
  else if(!unlocked)  {
    int fp=check_fingerprint();
    if(fp>0 && !unlocked) {
      fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_BLUE, 3);
      open_lock();
      log_activity(true, "from outside with fingerprint", fp);
    }
    else if(fp == 0)  {
      fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
      display_error();
      beep_nok();
      display_place_finger();
    }
  }

  if(digitalRead(BREACH_SENSOR) == HIGH)  {
    breach_alert();
    while(digitalRead(BREACH_SENSOR) == HIGH) beep_keypress();
  }

  ArduinoOTA.handle();
}
