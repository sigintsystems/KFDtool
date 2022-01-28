#ifndef DEFINES_H_
#define DEFINES_H_

// uncomment to enable the display (will require addt'l libraries)
#define DISPLAY_ENABLED

// THIS IS VERY VERY DANGEROUS
// uncomment to enable key readout; enables you to read locally stored keys back to the host
// DO NOT ENABLE THIS WITHOUT KNOWING WHAT YOU ARE DOING
#define ENABLE_KEY_READOUT

#define ACTIVITY_LED_PIN 7  // if building without the shield, change this to 13 for an activity indicator on the built-in LED
#define SENSE_LED_PIN 6
#define DATA_TX 5   // TWI Data TX
#define DATA_RX 3   // TWI Data RX (INT0)
#define SNS_TX 4    // TWI Sense TX
#define SNS_RX 2    // TWI Sense RX (INT1)
#define GPIO1 8
#define GPIO2 9
#define UP_BUTTON_PIN 14    // these should be left alone if you are using a shield
#define DOWN_BUTTON_PIN 16
#define ENTER_BUTTON_PIN 15

// the max number of keys that can be stored in eeprom
// each key takes up ~40 bytes of eeprom, plus the other eeprom data
#define MAX_KEYS 15

#endif