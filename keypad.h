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

const String offline_pin_hash="03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4";

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

  JsonObject& users = get_users();
  if(users.containsKey("users"))  {
    int user_id = 0;
    JsonArray& user_array = users["users"];

    Serial.println("Retrieved "+String(user_array.size())+" users, searching for a match");
    for(int i = 0 ; i < user_array.size() ; i++)  {
      if(user_array[i]["pin"] == enteredPinHash) return user_array[i]["id"];
    }
    
    return user_id;
  } else  {
    if(offline_pin_hash == enteredPinHash) {
      return 1;
    } else  {
      return -1;
    }
  }
}
