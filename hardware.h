/*****************************************************************************
 * 
 * Header file defining all functions related to the doorlock hardware (LEDs, 
 * button, servomotor).
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <ESP32Servo.h>
#include "display.h"
#include "buzzer.h"

#define SERVO_PIN 23
#define GREEN_LED 18
#define RED_LED 4
#define PUSH_BUTTON 15

Servo servo;
bool unlocked=false;

void init_hardware()  {
  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN, 0, 2500);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);
}

void open_lock()  {
  Serial.println("Unlocking door");
  display_lock();
  digitalWrite(GREEN_LED, HIGH);
  servo.write(180);
  unlocked=true;
  delay(400);
  digitalWrite(GREEN_LED, LOW);
  display_unlock();
}

void close_lock() {
  Serial.println("Locking door");
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
