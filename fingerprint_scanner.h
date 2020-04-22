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

Adafruit_Fingerprint fpScanner = Adafruit_Fingerprint(&Serial2);

//Initializes communication with the fingerprint scanner
void fingerprint_scanner_init() {
  fpScanner.begin(57600);
}

//Scans a new fingerprint (twice) and stores it
bool add_fingerprint()  {
  if(fpScanner.verifyPassword()) {
    Serial.println("Capteur biométrique détecté");
    int id=0;
    while(id==0)  {
      id=1;
    }
    Serial.println("Enregistrement de l'empreinte #"+String(id));
    int empreinte=-1;
    display_place_finger();
    while(empreinte != FINGERPRINT_OK)  {
      empreinte=fpScanner.getImage();
      switch(empreinte) {
        case FINGERPRINT_OK:
          Serial.println("Image prise");
          break;
        case FINGERPRINT_NOFINGER:
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Erreur de communication avec le capteur");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Erreur de capture");
          break;
        default:
          Serial.println("Erreur inconnue");
          break;
      }
    }
    
    empreinte=fpScanner.image2Tz();
    switch (empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image convertie");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Mauvaise qualité d'image");
        return false;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erreur de communication");
        return false;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Aucune empreinte détectée");
        return false;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Aucune empreinte détectée");
        return false;
      default:
        Serial.println("Erreur inconnue");
        return false;
    }

    Serial.println("Retirez votre doigt");
    display_remove_finger();
    empreinte=0;
    while(empreinte != FINGERPRINT_NOFINGER) {
      empreinte=fpScanner.getImage();
    }

    Serial.println("Placer le même doigt sur le capteur");
    display_place_finger();
    empreinte=-1;
    while(empreinte != FINGERPRINT_OK)  {
      empreinte=fpScanner.getImage();
      switch(empreinte) {
        case FINGERPRINT_OK:
          Serial.println("Image prise");
          break;
        case FINGERPRINT_NOFINGER:
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Erreur de communication avec le capteur");
          break;
        case FINGERPRINT_IMAGEFAIL:
          Serial.println("Erreur de capture");
          break;
        default:
          Serial.println("Erreur inconnue");
          break;
      }
    }
    
    empreinte=fpScanner.image2Tz();
    switch (empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Image convertie");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Mauvaise qualité d'image");
        return false;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erreur de communication");
        return false;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Aucune empreinte détectée");
        return false;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Aucune empreinte détectée");
        return false;
      default:
        Serial.println("Erreur inconnue");
        return false;
    }
    Serial.println("Retirez votre doigt");
    display_remove_finger();

    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Les empreintes sont identiques");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erreur de communication");
        return false;
        break;
      case FINGERPRINT_ENROLLMISMATCH:
        Serial.println("Les empreintes de coïncident pas");
        return false;
        break;
      default:
        Serial.println("Erreur inconnue");
        return false;
        break;
    }

    empreinte=fpScanner.storeModel(id);
    switch(empreinte) {
      case FINGERPRINT_OK:
        Serial.println("Empreinte enregistrée");
        return true;
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Erreur de communication");
        return false;
        break;
      case FINGERPRINT_BADLOCATION:
        Serial.println("Impossible d'enregistrer l'empreinte à cet endroit");
        return false;
        break;
      case FINGERPRINT_FLASHERR:
        Serial.println("Erreur d'écriture en mémoire flash");
        return false;
        break;
      default:
        Serial.println("Erreur inconnue");
        return false;
        break;
    }
  }
  else  {
    Serial.println("Impossible de se connecter au capteur biométrique");
    return false;
  }
}

//Scans a fingerprint and compares it to the stored ones
int check_fingerprint()
{
  int empreinte=fpScanner.getImage();
  switch(empreinte) {
    case FINGERPRINT_OK:
      Serial.println("Image prise");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Pas d'empreinte détectée");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erreur de communication");
      return -1;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erreur de capture");
      return -1;
    default:
      Serial.println("Erreur inconnue");
      return -1;
  }

  empreinte=fpScanner.image2Tz();
  switch(empreinte) {
    case FINGERPRINT_OK:
      Serial.println("Image convertie");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Mauvaise qualité d'image");
      return -1;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erreur de communication");
      return -1;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Aucune empreinte détectée");
      return -1;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Aucune empreinte détectée");
      return -1;
    default:
      Serial.println("Erreur inconnue");
      return -1;
  }

  empreinte=fpScanner.fingerFastSearch();
  switch(empreinte) {
    case FINGERPRINT_OK:
      if(fpScanner.confidence > 40) {
        Serial.println("Correspondance à l'empreinte "+String(fpScanner.fingerID)+" trouvée de "+String(fpScanner.confidence)+"%");
        return fpScanner.fingerID;
      } else  {
        Serial.println("Correspondance insuffisante");
        return 0;
      }
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erreur de communication");
      return -1;
      break;
    case FINGERPRINT_NOTFOUND:
      Serial.println("Aucune correspondance trouvée");
      return 0;
      break;
    default:
      Serial.println("Erreur inconnue");
      return -1;
      break;
  }
}
