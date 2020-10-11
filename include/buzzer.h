/*****************************************************************************
 * 
 * Header file defining all functions related to the sound interface (piezo).
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#define BUZZER_PIN 5
#define BUZZER_CHANNEL 4

void beep_ok() {
  ledcSetup(BUZZER_CHANNEL, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, 2000);
  delay(100);
  ledcDetachPin(BUZZER_PIN);
  delay(100);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, 2000);
  delay(100);
  ledcDetachPin(BUZZER_PIN);
}

void beep_nok() {
  ledcSetup(BUZZER_CHANNEL, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, 100);
  delay(1000);
  ledcDetachPin(BUZZER_PIN);
}

void beep_keypress()  {
  ledcSetup(BUZZER_CHANNEL, 2000, 8);
  ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
  ledcWriteTone(BUZZER_CHANNEL, 5000);
  delay(80);
  ledcDetachPin(BUZZER_PIN);
}
