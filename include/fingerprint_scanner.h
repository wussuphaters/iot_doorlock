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

#define LED_RING 18
#define TOUCH_INTERRUPT 34

Adafruit_Fingerprint fpScanner = Adafruit_Fingerprint(&Serial2);

//Initializes communication with the fingerprint scanner
void init_fingerprint_scanner() {
  fpScanner.begin(57600);
  pinMode(LED_RING, OUTPUT);
}

//Scans a new fingerprint (twice) and stores it
bool add_fingerprint(int user_id)  {
  if(fpScanner.verifyPassword()) {
    Serial.println("Fingerprint scanner detected");
    Serial.println("Enrolling fingerprint for user #"+String(user_id));
    int empreinte=-1;
    display_place_finger();
    digitalWrite(LED_RING, HIGH);
    while(empreinte != FINGERPRINT_OK)  {
      empreinte=fpScanner.getImage();
      switch(empreinte) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error with scanner");
          digitalWrite(LED_RING, LOW);
          return false;
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Capture error");
          digitalWrite(LED_RING, LOW);
          return false;
          break;
        default:
          Serial.println("Unknown error");
          break;
      }
    }
    
    empreinte=fpScanner.image2Tz();
    switch (empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Bad image quality");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("No fingerprint detected");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No fingerprint detected");
        digitalWrite(LED_RING, LOW);
        return false;
      default:
        Serial.println("Erreur inconnue");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
    }

    Serial.println("Remove finger");
    display_remove_finger();
    empreinte=0;
    while(empreinte != FINGERPRINT_NOFINGER) {
      empreinte=fpScanner.getImage();
    }

    Serial.println("Place same finger");
    display_place_finger();
    empreinte=-1;
    while(empreinte != FINGERPRINT_OK)  {
      empreinte=fpScanner.getImage();
      switch(empreinte) {
        case FINGERPRINT_OK:
          Serial.println("Image taken");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          digitalWrite(LED_RING, LOW);
          return false;
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Capture error");
          digitalWrite(LED_RING, LOW);
          return false;
          break;
      }
    }
    
    empreinte=fpScanner.image2Tz();
    switch (empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Bad image quality");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("No fingerprint detected");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No fingerprint detected");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      default:
        Serial.println("Unknown error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
    }
    Serial.println("Remove finger");
    display_remove_finger();

    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Fingerprints match");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_ENROLLMISMATCH:
        Serial.println("Fingerprints don't match");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      default:
        Serial.println("Unknown error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
    }

    empreinte=fpScanner.storeModel(user_id);
    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Fingerprint enrolled");
        digitalWrite(LED_RING, LOW);
        return true;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_BADLOCATION:
        Serial.println("Cannot store fingerprint for this user id");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      case FINGERPRINT_FLASHERR:
        Serial.println("Flash memory write error");
        digitalWrite(LED_RING, LOW);
        return false;
        break;
      default:
        Serial.println("Unknown error");
        digitalWrite(LED_RING, LOW);
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
        Serial.println("Image taken");
        digitalWrite(LED_RING, HIGH);
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
        digitalWrite(LED_RING, LOW);
        return -1;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        digitalWrite(LED_RING, LOW);
        return -1;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("No fingerprint detected");
        digitalWrite(LED_RING, LOW);
        return -1;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("No fingerprint detected");
        digitalWrite(LED_RING, LOW);
        return -1;
      default:
        Serial.println("Unknown error");
        digitalWrite(LED_RING, LOW);
        return -1;
    }
  
    empreinte=fpScanner.fingerFastSearch();
    switch(empreinte) {
      case FINGERPRINT_OK:
        if(fpScanner.confidence > 40) {
          Serial.println(String(fpScanner.confidence)+"% match to user id #"+String(fpScanner.fingerID));
          digitalWrite(LED_RING, LOW);
          return fpScanner.fingerID;
        } else  {
          Serial.println("Unsufficient confidence");
          digitalWrite(LED_RING, LOW);
          return 0;
        }
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        digitalWrite(LED_RING, LOW);
        return -1;
        break;
      case FINGERPRINT_NOTFOUND:
        Serial.println("No match found");
        digitalWrite(LED_RING, LOW);
        return 0;
        break;
      default:
        Serial.println("Unknown error");
        digitalWrite(LED_RING, LOW);
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