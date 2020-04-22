/*****************************************************************************
 * 
 * Main file for the IOT doorlock processing.
 * Uses the ESP32Servo library written by @hephaestus.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <ESP32Servo.h>
#include <WiFi.h>
#include "screen.h"
#include "fingerprint_scanner.h"
#include "buzzer.h"
#include "keypad.h"

#define SERVO_PIN 23
#define GREEN_LED 18
#define RED_LED 4
#define PUSH_BUTTON 15

#define WIFI_SSID "tuveuxdupoulet"
#define WIFI_PASSWORD "Mcwalt3risbacK"

Servo servo;
WiFiClient espClient;
bool unlocked=false;

void setup() {
  Serial.begin(115200);  
  connect_wifi();
  delay(100);
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 0, 2500);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);
  display_init();
  fingerprint_scanner_init();

  display_place_finger();
}

void loop() {
  reconnect_wifi();
  char key=numpad.getKey();

  if(key) {
    beep_keypress();
    if(key=='#' && unlocked) close_lock();
    else if(!unlocked && (key=='0' || key=='1' || key=='2' || key=='3' || key=='4' || key=='5' || key=='6' || key=='7' || key=='8' || key=='9' || key=='A' || key=='B' || key=='C' || key=='D'))  {
      String enteredPin="";

      while(key != '*' && enteredPin.length() <8) {
        if(key) {
          enteredPin+=key;
          display_pin(enteredPin.length());
        }
  
        key=numpad.getKey();
        if(key) beep_keypress();
      }
  
      if(enteredPin.length() > 0 && checkPin(enteredPin))  {
        Serial.println("Code bon");
        beep_ok();
        open_lock();
      } else  {
        display_error();
        beep_nok();
        display_place_finger();
      }
      enteredPin="";
    }
  }
  else if(digitalRead(PUSH_BUTTON)==HIGH) on_button_press();
  else  {
    int fp=check_fingerprint();
    if(fp>0 && !unlocked) {
      beep_ok();
      open_lock();
    }
    else if(fp == 0)  {
      display_error();
      beep_nok();
      display_place_finger();
    }
  }
}

void open_lock()  {
  Serial.println("Déverrouillage de la porte");
  display_lock();
  digitalWrite(GREEN_LED, HIGH);
  servo.write(180);
  unlocked=true;
  delay(400);
  digitalWrite(GREEN_LED, LOW);
  display_unlock();
}

void close_lock() {
  Serial.println("Vérrouillage de la porte");
  digitalWrite(RED_LED, HIGH);
  servo.write(0);
  unlocked=false;
  display_lock();
  delay(800);
  digitalWrite(RED_LED, LOW);
  display_place_finger();
}

void on_button_press()  {
  beep_keypress();
  if(!unlocked) open_lock();
  else close_lock();
}

void reconnect_wifi() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi déconnecté, tentative de reconnexion...");
    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    if(WiFi.status() != WL_CONNECTED) Serial.println("Succès");
    else Serial.println("Echec");
  }
}

void connect_wifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connexion WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("\nConnecté");
}
