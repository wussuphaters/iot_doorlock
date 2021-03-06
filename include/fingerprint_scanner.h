/*****************************************************************************
 * 
 * Header file defining all functions related to fingerprint scanner.
 * Uses the Adafruit Fingerprint library.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <Adafruit_Fingerprint.h>

#define ENROLL_TIMEOUT 30000

Adafruit_Fingerprint fpScanner = Adafruit_Fingerprint(&Serial2, 1337);

//Initializes communication with the fingerprint scanner
void init_fingerprint_scanner() {
  fpScanner.begin(57600);
  //fpScanner.setPassword(1337);
}

//Scans a new fingerprint (twice) and stores it
bool add_fingerprint(int uid)  {
  if(fpScanner.verifyPassword()) {
    Serial.println("Fingerprint scanner detected");
    Serial.println("Enrolling fingerprint for user #"+String(uid));
    int empreinte=-1;
    fpScanner.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
    display_place_finger();
    unsigned long startMillis=millis();
    while(empreinte != FINGERPRINT_OK)  {
      if(millis() > (startMillis + ENROLL_TIMEOUT) || millis() < startMillis) {
        Serial.println("Enroll timeout");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
      }
      empreinte=fpScanner.getImage();
      switch(empreinte) {
        case FINGERPRINT_NOFINGER:
          break;
        case FINGERPRINT_OK:
          fpScanner.LEDcontrol(FINGERPRINT_LED_BREATHING, 100, FINGERPRINT_LED_BLUE);
          Serial.println("Image taken");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error with sensor");
          fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
          return false;
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Capture error");
          fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
          return false;
          break;
        default:
          Serial.println("Unknown error");
          fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
          return false;
          break;
      }
    }
    
    empreinte=fpScanner.image2Tz(1);
    switch (empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Bad image quality");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("No fingerprint detected");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No fingerprint detected");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
      default:
        Serial.println("Erreur inconnue");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
    }

    Serial.println("Remove finger");
    fpScanner.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
    display_remove_finger();
    delay(2000);
    empreinte=0;
    startMillis = millis();
    while(empreinte != FINGERPRINT_NOFINGER) {
      if(millis() > (startMillis + ENROLL_TIMEOUT) || millis() < startMillis) {
        Serial.println("Enroll timeout");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
      }
      empreinte=fpScanner.getImage();
    }
    fpScanner.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
    Serial.println("Place same finger");
    display_place_finger();
    empreinte=-1;
    startMillis = millis();
    while(empreinte != FINGERPRINT_OK)  {
      if(millis() > (startMillis + ENROLL_TIMEOUT) || millis() < startMillis) {
        Serial.println("Enroll timeout");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
      }
      empreinte=fpScanner.getImage();
      switch(empreinte) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          fpScanner.LEDcontrol(FINGERPRINT_LED_BREATHING, 100, FINGERPRINT_LED_BLUE);
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
          return false;
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Capture error");
          fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
          return false;
          break;
      }
    }
    
    empreinte=fpScanner.image2Tz(2);
    switch (empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Bad image quality");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("No fingerprint detected");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No fingerprint detected");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      default:
        Serial.println("Unknown error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
    }
    Serial.println("Remove finger");
    fpScanner.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
    display_remove_finger();

    empreinte = fpScanner.createModel();
    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Fingerprints match");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_ENROLLMISMATCH:
        Serial.println("Fingerprints don't match");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      default:
        Serial.println("Unknown error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
    }

    empreinte=fpScanner.storeModel(uid);
    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Fingerprint enrolled");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_BLUE, 3);
        return true;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_BADLOCATION:
        Serial.println("Cannot store fingerprint for this user id");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      case FINGERPRINT_FLASHERR:
        Serial.println("Flash memory write error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
      default:
        Serial.println("Unknown error");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return false;
        break;
    }
  } else  {
    Serial.println("Could not connect to fingerprint scanner");
    return false;
  }
}

//Scans a fingerprint and compares it to the stored ones
int check_fingerprint()
{
  if(fpScanner.verifyPassword()) {
    int empreinte=fpScanner.getImage();
    switch(empreinte) {
      case FINGERPRINT_OK:
        fpScanner.LEDcontrol(FINGERPRINT_LED_BREATHING, 100, FINGERPRINT_LED_BLUE);
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        return -1;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return -1;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Capture error");
        return -1;
      default:
        Serial.println("Unknown error");
        return -1;
    }
  
    empreinte=fpScanner.image2Tz();
    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Basd image quality");
        return -1;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return -1;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("No fingerprint detected");
        return -1;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No fingerprint detected");
        return -1;
      default:
        Serial.println("Unknown error");
        return -1;
    }
  
    empreinte=fpScanner.fingerSearch();
    switch(empreinte) {
      case FINGERPRINT_OK:
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_BLUE, 3);
        return fpScanner.fingerID;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return -1;
        break;
      case FINGERPRINT_NOTFOUND:
        Serial.println("No match found");
        fpScanner.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 3);
        return 0;
        break;
      default:
        Serial.println("Unknown error");
        return -1;
        break;
    }
  } else  {
    Serial.println("Could not connect to fingerprint scanner");
    return false;
  }
}

//Checks if a fingerprint corresponds to a user id
bool is_fingerprint_registered(int user_id)  {
  if(fpScanner.loadModel(user_id) == FINGERPRINT_OK) return true;
  else return false;
}

//Updates stored fingerprints (eg if user is in database deleted)
int delete_fingerprint(int user_id)  {
  Serial.println("Deleting fingerprint corresponding to user id #"+String(user_id)+"...");
  int tries = 0;
  int res;

  while(res = fpScanner.deleteModel(user_id) != FINGERPRINT_OK && tries < 3)  {
    tries++;
    Serial.println("Try "+String(tries)+" to delete fringerprint failed");
  }

  if(res == FINGERPRINT_OK) return true;
  return false;
}
