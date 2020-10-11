/*****************************************************************************
 * 
 * Header file defining all functions related to the numeric keypad.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */

#include <Keypad.h>
#include "api.h"
#include "mbedtls/md.h"

const String offline_pin_hash="d49768af3021632108831e387f00737e654ad0f3e4d3bd418c631ec2abc2c54b";

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'D','#','0','*'},
  {'C','9','8','7'},
  {'B','6','5','4'},
  {'A','3','2','1'}
};
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

Keypad numpad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//Hashes the PIN entered by the user and compares it to the authorized stored hashed PIN
int checkPin(String enteredPin) {
  size_t enteredPinLength=enteredPin.length();
  byte enteredPinHashArray[32];
  String enteredPinHash="";
  
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *)enteredPin.c_str(), enteredPinLength);
  mbedtls_md_finish(&ctx, enteredPinHashArray);
  mbedtls_md_free(&ctx);
  
  for(int i= 0; i< sizeof(enteredPinHashArray); i++)  {
    char str[3];
    sprintf(str, "%02x", (int)enteredPinHashArray[i]);
    enteredPinHash+=String(str);
  }

  StaticJsonDocument<5000> users = get_users();
  if(users.containsKey("pins"))  {
    int user_id = 0;
    JsonArray pin_array = users["pins"].as<JsonArray>();

    Serial.println("Retrieved "+String(pin_array.size())+" user pins, searching for a match");
    for(JsonVariant v : pin_array) {
        if(v["pin"] == enteredPinHash) return v["id"].as<int>();
    }
    
    return user_id;
  } else  {
    if(offline_pin_hash == enteredPinHash) {
      return 1;
    } else  {
      if(init_wifi()) return checkPin(enteredPin);
      else return -1;
    }
  }
}
