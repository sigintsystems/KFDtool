#ifndef HMI_H_
#define HMI_H_

#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET 17 // unused pin

#define SCREEN_ADDRESS 0x3c ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

void hmiUpdateMode(uint8_t mode);;
void hmiInitDisplay();
void hmiClearDisplay();
void hmiWriteAtLocation(uint8_t x, uint8_t y, const char * text);
void hmiRedrawMenus(uint8_t mode);
void hmiDoSplash();

#endif
