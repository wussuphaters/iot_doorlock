/*****************************************************************************
 * 
 * Header file defining all functions related to the I2C OLED screen.
 * Uses the Adafruit SSD1306 library.
 * 
 * Written by Maël PONCHANT
 * 
 * ***************************************************************************
 */
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LOGO_HEIGHT   64
#define LOGO_WIDTH    64
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

byte display_adress=0x3C;

static const unsigned char PROGMEM lock_logo[] =  {
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xe0, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf0, 0x00, 
  0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x00, 
  0x00, 0x00, 0x00, 0x7f, 0xfc, 0x7f, 0xfc, 0x00, 
  0x00, 0x00, 0x00, 0x7f, 0xe0, 0x0f, 0xfc, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0x80, 0x03, 0xfe, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0x00, 0x01, 0xfe, 0x00, 
  0x00, 0x00, 0x01, 0xfe, 0x00, 0x01, 0xfe, 0x00, 
  0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0xff, 0x00, 
  0x00, 0x00, 0x01, 0xfe, 0x00, 0x00, 0xff, 0x00, 
  0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x7f, 0x00, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x00, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x00, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x00, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x80, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x80, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x80, 
  0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x7f, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0
};

static const unsigned char PROGMEM unlock_logo[] =  {
  0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 
  0x03, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x07, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x0f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 
  0x1f, 0xff, 0x1f, 0xfe, 0x00, 0x00, 0x00, 0x00, 
  0x3f, 0xf0, 0x03, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x3f, 0xe0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 
  0x3f, 0xc0, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 
  0x7f, 0x80, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00, 
  0x7f, 0x80, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
  0x7f, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xff, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xf0, 
  0xff, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xe0, 0x0f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfc, 
  0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 
  0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0
};

static const unsigned char PROGMEM place_finger_logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x0f, 0xe0, 0x1f, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x00, 0x00, 0xf8, 0x00, 0x00, 
  0x00, 0x00, 0xc0, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0x80, 0x01, 0xf0, 0x00, 0xc0, 0x00, 
  0x00, 0x0f, 0x01, 0xf1, 0xff, 0x00, 0xe0, 0x00, 
  0x00, 0x1e, 0x07, 0xf0, 0xff, 0xc0, 0x78, 0x00, 
  0x00, 0x3c, 0x1f, 0x80, 0x03, 0xf0, 0x38, 0x00, 
  0x00, 0x38, 0x3c, 0x00, 0x00, 0x7c, 0x1c, 0x00, 
  0x00, 0x70, 0x78, 0x00, 0x00, 0x1e, 0x0e, 0x00, 
  0x00, 0xe0, 0xe0, 0x03, 0xc0, 0x07, 0x07, 0x00, 
  0x01, 0xc3, 0xc0, 0x3f, 0xf8, 0x03, 0xc3, 0x80, 
  0x01, 0xc3, 0x80, 0xff, 0xf8, 0x81, 0xc3, 0x80, 
  0x03, 0x87, 0x03, 0xf0, 0x01, 0xe0, 0xe1, 0xc0, 
  0x07, 0x0e, 0x07, 0x80, 0x00, 0xf0, 0x71, 0xc0, 
  0x0f, 0x0e, 0x07, 0x00, 0x00, 0x78, 0x30, 0xe0, 
  0x0e, 0x1c, 0x00, 0x00, 0x00, 0x3c, 0x38, 0xe0, 
  0x0c, 0x38, 0x00, 0x0f, 0xf0, 0x1e, 0x1c, 0x70, 
  0x00, 0x30, 0x70, 0x3f, 0xfc, 0x0e, 0x1c, 0x70, 
  0x00, 0x00, 0x70, 0xfc, 0x3f, 0x07, 0x0c, 0x38, 
  0x00, 0x00, 0xe1, 0xe0, 0x07, 0x83, 0x8e, 0x38, 
  0x00, 0x81, 0xc3, 0xc0, 0x03, 0xc3, 0x8e, 0x38, 
  0x01, 0xc1, 0xc3, 0x80, 0x01, 0xc1, 0x86, 0x18, 
  0x03, 0x83, 0x87, 0x0e, 0x70, 0xe1, 0xc7, 0x1c, 
  0x07, 0x03, 0x8e, 0x1e, 0x38, 0xe0, 0xc7, 0x1c, 
  0x1f, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xc7, 0x1c, 
  0x3c, 0x0e, 0x0c, 0x38, 0x1c, 0x70, 0xe3, 0x0c, 
  0x38, 0x0e, 0x00, 0x30, 0x1c, 0x20, 0xe3, 0x0c, 
  0x00, 0x1c, 0x00, 0x71, 0x8c, 0x00, 0xe3, 0x0e, 
  0x00, 0x38, 0x00, 0x71, 0x8c, 0x00, 0xe3, 0x0c, 
  0x00, 0x70, 0x60, 0x61, 0x8c, 0x00, 0x63, 0x80, 
  0x00, 0xe0, 0xe0, 0xe3, 0x8c, 0x38, 0x63, 0x80, 
  0x3f, 0xc0, 0xe0, 0xe3, 0x8c, 0x38, 0x63, 0x80, 
  0x3f, 0x81, 0xc0, 0xc3, 0x8c, 0x38, 0x63, 0x80, 
  0x1c, 0x03, 0xc1, 0xc3, 0x1c, 0x38, 0x63, 0x80, 
  0x00, 0x07, 0x81, 0x87, 0x1c, 0x38, 0xe3, 0x80, 
  0x00, 0x0f, 0x03, 0x87, 0x1c, 0x38, 0xe3, 0x80, 
  0x00, 0x1e, 0x03, 0x86, 0x18, 0x30, 0xe3, 0x00, 
  0x07, 0xfc, 0x07, 0x0e, 0x38, 0x30, 0xe3, 0x00, 
  0x0f, 0xf0, 0x0e, 0x0e, 0x38, 0x70, 0xc3, 0x00, 
  0x0f, 0x80, 0x1c, 0x00, 0x38, 0x70, 0xc7, 0x00, 
  0x00, 0x00, 0x3c, 0x00, 0x70, 0x70, 0x07, 0x00, 
  0x00, 0x00, 0x78, 0x30, 0x70, 0x60, 0x07, 0x00, 
  0x00, 0x01, 0xe0, 0x70, 0x60, 0xe0, 0x06, 0x00, 
  0x00, 0x0f, 0xc0, 0x70, 0xe0, 0xe1, 0x86, 0x00, 
  0x00, 0xff, 0x80, 0xe0, 0xe0, 0xc1, 0x80, 0x00, 
  0x00, 0xfc, 0x01, 0xc1, 0xc1, 0xc3, 0x80, 0x00, 
  0x00, 0x00, 0x03, 0x81, 0xc1, 0xc3, 0x80, 0x00, 
  0x00, 0x00, 0x07, 0x03, 0x83, 0x83, 0x00, 0x00, 
  0x00, 0x00, 0x1e, 0x07, 0x03, 0x87, 0x00, 0x00, 
  0x00, 0x00, 0x7c, 0x07, 0x07, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0xf8, 0x0e, 0x07, 0x06, 0x00, 0x00, 
  0x00, 0x01, 0xe0, 0x1c, 0x0e, 0x06, 0x00, 0x00, 
  0x00, 0x00, 0x80, 0x38, 0x0e, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x78, 0x1c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xe0, 0x38, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xc0, 0x38, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char PROGMEM remove_finger_logo[] =  {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x0f, 0xe0, 0x0f, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x00, 0x00, 0xf8, 0x00, 0x00, 
  0x00, 0x00, 0xc0, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0x80, 0x01, 0xf0, 0x00, 0xc0, 0x00, 
  0x00, 0x0f, 0x01, 0xf1, 0xff, 0x00, 0xf0, 0x00, 
  0x00, 0x1e, 0x07, 0xf0, 0xff, 0xe0, 0x78, 0x00, 
  0x00, 0x1c, 0x1f, 0x80, 0x03, 0xf8, 0x3c, 0x00, 
  0x00, 0x38, 0x3c, 0x00, 0x00, 0x7c, 0x1e, 0x00, 
  0x00, 0x70, 0x78, 0x00, 0x00, 0x1f, 0x0e, 0x00, 
  0x00, 0xe0, 0xe0, 0x03, 0xe0, 0x07, 0x87, 0x00, 
  0x01, 0xe1, 0xc0, 0x3f, 0xf8, 0x03, 0xc3, 0x80, 
  0x01, 0xc3, 0x80, 0xff, 0xf8, 0x81, 0xc3, 0x80, 
  0x03, 0x87, 0x03, 0xf0, 0x01, 0xe0, 0xe1, 0xc0, 
  0x07, 0x0e, 0x07, 0x80, 0x00, 0xf0, 0x71, 0xe0, 
  0x0f, 0x0e, 0x07, 0x00, 0x00, 0x78, 0x38, 0xe0, 
  0x0e, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x38, 0x60, 
  0x0c, 0x38, 0x00, 0x0f, 0xf0, 0x0e, 0x1c, 0x70, 
  0x00, 0x38, 0x30, 0x3f, 0xfc, 0x0f, 0x1c, 0x70, 
  0x00, 0x00, 0x70, 0xfc, 0x3e, 0x07, 0x0e, 0x38, 
  0x00, 0x00, 0xe1, 0xe0, 0x07, 0x03, 0x8e, 0x38, 
  0x00, 0x80, 0xe3, 0xc0, 0x03, 0x83, 0x86, 0x18, 
  0x01, 0xc1, 0xc3, 0x80, 0x01, 0xc1, 0xc6, 0x1c, 
  0x03, 0x83, 0x87, 0x0e, 0x30, 0xe1, 0xc7, 0x1c, 
  0x07, 0x03, 0x8e, 0x1e, 0x38, 0xe0, 0xc7, 0x1c, 
  0x1f, 0x07, 0x0e, 0x1c, 0x3c, 0x70, 0xe7, 0x0c, 
  0x3c, 0x0e, 0x0c, 0x38, 0x1c, 0x70, 0xe3, 0x0c, 
  0x38, 0x0e, 0x00, 0x38, 0x0c, 0x30, 0xe3, 0x0e, 
  0x00, 0x1c, 0x00, 0x70, 0x8c, 0x30, 0x63, 0x8e, 
  0x00, 0x38, 0x00, 0x71, 0x8c, 0x38, 0x63, 0x8c, 
  0x00, 0x78, 0x70, 0x61, 0x8c, 0x38, 0x63, 0x80, 
  0x00, 0xf0, 0x70, 0xe1, 0x8c, 0x38, 0x03, 0x00, 
  0x3f, 0xc0, 0xe0, 0xe3, 0x8c, 0x38, 0x00, 0x00, 
  0x3f, 0x81, 0xc0, 0xc3, 0x8c, 0x00, 0x30, 0x00, 
  0x1c, 0x01, 0xc1, 0xc3, 0x0c, 0x03, 0xff, 0x00, 
  0x00, 0x07, 0x81, 0xc7, 0x1c, 0x0f, 0xff, 0xc0, 
  0x00, 0x0f, 0x03, 0x87, 0x1c, 0x1f, 0x03, 0xe0, 
  0x00, 0x1e, 0x03, 0x86, 0x1c, 0x3c, 0x00, 0x70, 
  0x03, 0xfc, 0x07, 0x06, 0x18, 0x78, 0x00, 0x38, 
  0x0f, 0xf0, 0x0e, 0x0e, 0x38, 0xf0, 0x00, 0x1c, 
  0x0f, 0x80, 0x1e, 0x00, 0x38, 0xe0, 0x00, 0x0e, 
  0x00, 0x00, 0x3c, 0x00, 0x31, 0xc0, 0x00, 0x0e, 
  0x00, 0x00, 0x78, 0x30, 0x71, 0xc0, 0x00, 0x06, 
  0x00, 0x01, 0xf0, 0x30, 0x71, 0x80, 0x00, 0x07, 
  0x00, 0x0f, 0xc0, 0x70, 0xe1, 0x80, 0x00, 0x07, 
  0x00, 0xff, 0x80, 0xe0, 0xe3, 0x8f, 0xff, 0xe7, 
  0x00, 0xfc, 0x01, 0xc1, 0xc3, 0x8f, 0xff, 0xe7, 
  0x00, 0x00, 0x03, 0xc1, 0xc1, 0x80, 0x00, 0x07, 
  0x00, 0x00, 0x07, 0x83, 0x81, 0x80, 0x00, 0x07, 
  0x00, 0x00, 0x1f, 0x03, 0x81, 0xc0, 0x00, 0x07, 
  0x00, 0x00, 0x3c, 0x07, 0x01, 0xc0, 0x00, 0x06, 
  0x00, 0x00, 0xf8, 0x0e, 0x00, 0xc0, 0x00, 0x0e, 
  0x00, 0x01, 0xe0, 0x1c, 0x00, 0xe0, 0x00, 0x1c, 
  0x00, 0x00, 0x80, 0x3c, 0x00, 0x70, 0x00, 0x3c, 
  0x00, 0x00, 0x00, 0x78, 0x00, 0x38, 0x00, 0x78, 
  0x00, 0x00, 0x00, 0xe0, 0x00, 0x1e, 0x00, 0xf0, 
  0x00, 0x00, 0x00, 0xc0, 0x00, 0x0f, 0xff, 0xe0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00
};

static const unsigned char PROGMEM finger_accepted_logo[] =  {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x0f, 0xe0, 0x0f, 0xf0, 0x00, 0x00, 
  0x00, 0x00, 0x0e, 0x00, 0x00, 0xf8, 0x00, 0x00, 
  0x00, 0x00, 0xc0, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x07, 0x80, 0x01, 0xf0, 0x00, 0xc0, 0x00, 
  0x00, 0x0f, 0x01, 0xf1, 0xff, 0x00, 0xf0, 0x00, 
  0x00, 0x1e, 0x07, 0xf0, 0xff, 0xe0, 0x78, 0x00, 
  0x00, 0x1c, 0x1f, 0x80, 0x03, 0xf8, 0x3c, 0x00, 
  0x00, 0x38, 0x3c, 0x00, 0x00, 0x7c, 0x1e, 0x00, 
  0x00, 0x70, 0x78, 0x00, 0x00, 0x1f, 0x0e, 0x00, 
  0x00, 0xe0, 0xe0, 0x03, 0xe0, 0x07, 0x87, 0x00, 
  0x01, 0xe1, 0xc0, 0x3f, 0xf8, 0x03, 0xc3, 0x80, 
  0x01, 0xc3, 0x80, 0xff, 0xf8, 0x81, 0xc3, 0x80, 
  0x03, 0x87, 0x03, 0xf0, 0x01, 0xe0, 0xe1, 0xc0, 
  0x07, 0x0e, 0x07, 0x80, 0x00, 0xf0, 0x71, 0xe0, 
  0x0f, 0x0e, 0x07, 0x00, 0x00, 0x78, 0x38, 0xe0, 
  0x0e, 0x1c, 0x00, 0x00, 0x00, 0x1c, 0x38, 0x60, 
  0x0c, 0x38, 0x00, 0x0f, 0xf0, 0x0e, 0x1c, 0x70, 
  0x00, 0x38, 0x30, 0x3f, 0xfc, 0x0f, 0x1c, 0x70, 
  0x00, 0x00, 0x70, 0xfc, 0x3e, 0x07, 0x0e, 0x38, 
  0x00, 0x00, 0xe1, 0xe0, 0x07, 0x03, 0x8e, 0x38, 
  0x00, 0x80, 0xe3, 0xc0, 0x03, 0x83, 0x86, 0x18, 
  0x01, 0xc1, 0xc3, 0x80, 0x01, 0xc1, 0xc6, 0x1c, 
  0x03, 0x83, 0x87, 0x0e, 0x30, 0xe1, 0xc7, 0x1c, 
  0x07, 0x03, 0x8e, 0x1e, 0x38, 0xe0, 0xc7, 0x1c, 
  0x1f, 0x07, 0x0e, 0x1c, 0x3c, 0x70, 0xe7, 0x0c, 
  0x3c, 0x0f, 0x0c, 0x38, 0x1c, 0x70, 0xe3, 0x0c, 
  0x38, 0x0e, 0x00, 0x38, 0x1c, 0x30, 0xe3, 0x0e, 
  0x00, 0x1c, 0x00, 0x70, 0x8c, 0x30, 0x63, 0x8e, 
  0x00, 0x38, 0x00, 0x71, 0x8c, 0x38, 0x63, 0x8c, 
  0x00, 0x78, 0x70, 0x61, 0x8c, 0x38, 0x63, 0x80, 
  0x00, 0xf0, 0x70, 0xe1, 0x8c, 0x38, 0x03, 0x00, 
  0x3f, 0xc0, 0xe0, 0xe3, 0x8c, 0x38, 0x00, 0x00, 
  0x3f, 0x81, 0xc0, 0xc3, 0x8c, 0x00, 0x30, 0x00, 
  0x1c, 0x01, 0xc1, 0xc3, 0x0c, 0x03, 0xff, 0x00, 
  0x00, 0x07, 0x81, 0xc7, 0x1c, 0x0f, 0xff, 0xc0, 
  0x00, 0x0f, 0x03, 0x87, 0x1c, 0x1f, 0x03, 0xe0, 
  0x00, 0x1e, 0x03, 0x86, 0x1c, 0x3c, 0x00, 0xf0, 
  0x03, 0xfc, 0x07, 0x06, 0x18, 0x78, 0x00, 0x38, 
  0x0f, 0xf0, 0x0e, 0x0e, 0x38, 0xf0, 0x00, 0x1c, 
  0x0f, 0x80, 0x1e, 0x00, 0x38, 0xe0, 0x00, 0x0e, 
  0x00, 0x00, 0x3c, 0x00, 0x31, 0xc0, 0x00, 0xce, 
  0x00, 0x00, 0x78, 0x30, 0x71, 0xc0, 0x01, 0xc6, 
  0x00, 0x01, 0xf0, 0x30, 0x71, 0x80, 0x03, 0x87, 
  0x00, 0x0f, 0xc0, 0x70, 0xe1, 0x80, 0x07, 0x07, 
  0x00, 0xff, 0x80, 0xe0, 0xe3, 0x80, 0x0f, 0x07, 
  0x00, 0xfc, 0x01, 0xc1, 0xc3, 0x86, 0x1e, 0x07, 
  0x00, 0x00, 0x03, 0xc1, 0xc1, 0x87, 0x9c, 0x07, 
  0x00, 0x00, 0x07, 0x83, 0x81, 0x83, 0xf8, 0x07, 
  0x00, 0x00, 0x1f, 0x03, 0x81, 0xc1, 0xf0, 0x06, 
  0x00, 0x00, 0x3c, 0x07, 0x01, 0xc0, 0xe0, 0x06, 
  0x00, 0x00, 0xf8, 0x0e, 0x00, 0xc0, 0x00, 0x0e, 
  0x00, 0x01, 0xe0, 0x1c, 0x00, 0xe0, 0x00, 0x1c, 
  0x00, 0x00, 0x80, 0x3c, 0x00, 0x70, 0x00, 0x3c, 
  0x00, 0x00, 0x00, 0x78, 0x00, 0x38, 0x00, 0x78, 
  0x00, 0x00, 0x00, 0xe0, 0x00, 0x1e, 0x00, 0xf0, 
  0x00, 0x00, 0x00, 0xc0, 0x00, 0x0f, 0xff, 0xe0, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00
};

static const unsigned char PROGMEM error_logo[] =  {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x1f, 0xe0, 0x07, 0xf8, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x00, 
  0x00, 0x00, 0xf8, 0x00, 0x00, 0x1f, 0x00, 0x00, 
  0x00, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x00, 
  0x00, 0x07, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 
  0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 
  0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 
  0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 
  0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 
  0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 
  0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 
  0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 
  0x06, 0x00, 0x06, 0x00, 0x00, 0x60, 0x00, 0x60, 
  0x0e, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x70, 
  0x0e, 0x00, 0x03, 0x80, 0x01, 0xc0, 0x00, 0x70, 
  0x1c, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x38, 
  0x1c, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x38, 
  0x1c, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x38, 
  0x18, 0x00, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x38, 
  0x18, 0x00, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x18, 
  0x18, 0x00, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x18, 
  0x1c, 0x00, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x38, 
  0x1c, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x38, 
  0x1c, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x38, 
  0x1c, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00, 0x30, 
  0x0e, 0x00, 0x03, 0x80, 0x01, 0xc0, 0x00, 0x70, 
  0x0e, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x70, 
  0x06, 0x00, 0x06, 0x00, 0x00, 0x60, 0x00, 0x60, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 
  0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 
  0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 
  0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 
  0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 
  0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 
  0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 
  0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 
  0x00, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 
  0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 
  0x00, 0x07, 0x80, 0x00, 0x00, 0x01, 0xe0, 0x00, 
  0x00, 0x03, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0x00, 
  0x00, 0x00, 0xf8, 0x00, 0x00, 0x1f, 0x00, 0x00, 
  0x00, 0x00, 0x7f, 0x00, 0x00, 0xfe, 0x00, 0x00, 
  0x00, 0x00, 0x1f, 0xf0, 0x0f, 0xf8, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

bool init_display() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, display_adress)) {
    Serial.println(F("Failed to connect to display"));
    return false;
  } else return true;
}

void display_place_finger()  {
  display.clearDisplay();
  display.drawBitmap(32, 0, place_finger_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void display_remove_finger()  {
  display.clearDisplay();
  display.drawBitmap(32, 0, remove_finger_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void display_finger_accepted()  {
  display.clearDisplay();
  display.drawBitmap(32, 0, finger_accepted_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void display_lock() {
  display.clearDisplay();
  display.drawBitmap(32, 0, lock_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void display_unlock() {
  display.clearDisplay();
  display.drawBitmap(32, 0, unlock_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void display_error()  {
  display.clearDisplay();
  display.drawBitmap(32, 0, error_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}

void display_pin(int stars)  {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  for(int i=0;i<stars;i++) {
    display.setCursor(4+16*i,32);
    display.println("*");
  }
  
  display.display();
}

void display_text(String msg) {
  display.clearDisplay();
  //display.drawBitmap(32, 0, network_error_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(msg);
  display.display();
}
