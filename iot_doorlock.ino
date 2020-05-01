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
#include "keypad.h"
#include "mqtt.h"

void setup() {
  Serial.begin(115200);
  init_display();
  display_text("Booting up")
  init_wifi();
  init_mqtt();
  init_hardware();
  init_fingerprint_scanner();

  display_place_finger();
}

void loop() {
  reconnect_wifi();
  reconnect_mqtt();
  char key=numpad.getKey();

  if(key) {
    beep_keypress();
    if(key=='#' && unlocked) close_lock();
    else if(!unlocked && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9' || key=='A' || key=='B' || key=='C' || key=='D'))  {
      String enteredPin="";

      while(key != '*') {
        if(key && enteredPin.length() < 8) {
          enteredPin+=key;
          display_pin(enteredPin.length());
        }
  
        key=numpad.getKey();
        if(key) beep_keypress();
      }
      if(enteredPin.length() > 0) {
        display_text("Wait...");
        int user_id = checkPin(enteredPin);
        if(user_id > 1)  {
          Serial.println("Access granted to user #"+String(user_id));
          /*
          if(!is_fingerprint_registered(user_id))  {
            Serial.println("User has no registered fingerprint, registering");
            int tries = 0;
            while(!add_fingerprint(user_id) && tries < 3)  {
              tries++;
              Serial.println("Try "+String(tries)+" to register fingerprint failed");
            }
          }
          */
          beep_ok();
          open_lock();
          notify(user_id);
        } else if(user_id == 1) {
          Serial.println("Master password used, access granted");
          beep_ok();
          open_lock();
          notify(user_id);
        } else if(user_id == 0)  {
          Serial.println("No pin match found, access denied");
          display_error();
          beep_nok();
          display_place_finger();
        } else  {
          Serial.println("User list could not be retrieved, master password must be used");
          display_utext("Please use\nmaster pin");
          beep_nok();
          display_place_finger();
        }
        enteredPin="";
      }
    }
  }
  else if(digitalRead(PUSH_BUTTON)==HIGH) on_button_press();
  else  {
    /*int fp=check_fingerprint();
    if(fp>0 && !unlocked) {
      beep_ok();
      open_lock();
    }
    else if(fp == 0)  {
      display_error();
      beep_nok();
      display_place_finger();
    }*/
  }
}
