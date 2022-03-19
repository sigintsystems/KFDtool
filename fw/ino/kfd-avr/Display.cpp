#include "Display.h"
#include "Defines.h"
#include "hal.h"

#ifdef DISPLAY_ENABLED
#include "modes/usb.h"
#include "modes/usb.cpp"
#include "p25.h"
#include "crc16.h"
#include "DisplayStrings.h"
#include <lcdgfx.h>
#include <lcdgfx_gui.h>
#include <avr/pgmspace.h>


#define BUTTON_UP_PRESSED digitalRead(UP_BUTTON_PIN) == 0
#define BUTTON_UP_RELEASED digitalRead(UP_BUTTON_PIN) == 1

#define BUTTON_DOWN_PRESSED digitalRead(DOWN_BUTTON_PIN) == 0
#define BUTTON_DOWN_RELEASED digitalRead(DOWN_BUTTON_PIN) == 1

#define BUTTON_ENTER_PRESSED digitalRead(ENTER_BUTTON_PIN) == 0
#define BUTTON_ENTER_RELEASED digitalRead(ENTER_BUTTON_PIN) == 1

#define MODE_MAIN_MENU 99
#define MODE_PC 0
#define MODE_KEY_FILL 1
#define MODE_KEY_ERASE 2
#define MODE_VIEW_KEY_INFO 3
#define MODE_VIEW_KEYSET 4
#define MODE_ACTIVATE_KEYSET 5
#define MODE_VIEW_INFO 6
#define MODE_ZEROIZE 7

uint8_t up_old_state = 0;
uint8_t down_old_state = 0;
uint8_t enter_old_state = 0;

uint8_t mode_stage = 0;

// generic buffer for sprintf'ing on the screen
char buf[40];

const char *menuItems[] =
{
    "PC Mode",
    "Key Fill",
    "Key Erase",
    "View Key Info",
    "View Keyset",
    "Activate Keyset",
    "View Info",
    "Zeroize Device",
};

uint8_t current_mode = MODE_PC; // start in main menu

DisplaySSD1306_128x64_I2C display(-1);

LcdGfxMenu menu(menuItems, sizeof(menuItems) / sizeof(char *));


void displayInit(void)
{
    display.setFixedFont(ssd1306xled_font6x8);

    display.begin();

    /* Uncomment 2 lines below to rotate your ssd1306 display by 180 degrees. */
    // display.getInterface().flipVertical();
    // display.getInterface().flipHorizontal();

    display.clear();
    menu.show( display );
}

void displayLoop(void){
    switch (current_mode)
    {
        case MODE_PC:
            // PC/host control mode
            usbModeLoop();
            if (BUTTON_ENTER_PRESSED && enter_old_state == 0) {
                enter_old_state = 1;
                onModeExit();
                current_mode = MODE_MAIN_MENU;
                onModeEntry();
            } else if (BUTTON_ENTER_RELEASED && enter_old_state == 1) {
                enter_old_state = 0;
            }
            break;

        case MODE_VIEW_INFO:
            if (BUTTON_ENTER_PRESSED && enter_old_state == 0) {
                enter_old_state = 1;
                onModeExit();
                current_mode = MODE_MAIN_MENU;
                onModeEntry();
            } else if (BUTTON_ENTER_RELEASED && enter_old_state == 1) {
                enter_old_state = 0;
            }
            break;

        case MODE_KEY_ERASE:
            switch(mode_stage) {
                case 0:
                    // first screen confirms erase keys
                    if (BUTTON_ENTER_PRESSED && enter_old_state == 0) {
                        display.clear();
                        display.printFixed(0, 32, "Erasing...", STYLE_NORMAL);
                        enter_old_state = 1;
                        // increment the stage
                        mode_stage++;
                        uint8_t result = p25EraseAllKeys();

                        if (result == 0) {
                            display.clear();
                            display.printFixed(0, 16, "Key erase success.", STYLE_NORMAL);
                            display.printFixed(0, 32, "Enter to exit...", STYLE_NORMAL);
                        } else {
                            sprintf(buf, "Error %d", result);
                            display.clear();
                            display.printFixed(0, 16, "Key erase fail.", STYLE_NORMAL);
                            display.printFixed(0, 32, buf, STYLE_NORMAL);
                            display.printFixed(0, 40, "Enter to exit...", STYLE_NORMAL);
                        }
                    } else if (BUTTON_ENTER_RELEASED && enter_old_state == 1) {
                        enter_old_state = 0;
                    }
                    break;
                case 1:
                    // second screen returns to home
                    if (BUTTON_ENTER_PRESSED && enter_old_state == 0) {
                        enter_old_state = 1;
                        onModeExit();
                        current_mode = MODE_MAIN_MENU;
                        onModeEntry();
                    } else if (BUTTON_ENTER_RELEASED && enter_old_state == 1) {
                        enter_old_state = 0;
                    }
                    break;
                default:
                    break;
            }
            
            break;

        case MODE_MAIN_MENU:
            // main menu
            mainMenuLoop();

        default:
            break;
    }

}

void mainMenuLoop(void) {
    if (BUTTON_UP_PRESSED && up_old_state == 0) {
        up_old_state = 1;
        menu.up();
        menu.show(display);
    } else if (BUTTON_UP_RELEASED && up_old_state == 1) {
        up_old_state = 0;
    }

    if (BUTTON_DOWN_PRESSED && down_old_state == 0) {
        down_old_state = 1;
        menu.down();
        menu.show(display);
    } else if (BUTTON_DOWN_RELEASED && down_old_state == 1) {
        down_old_state = 0;
    }

    if (BUTTON_ENTER_PRESSED && enter_old_state == 0) {
        enter_old_state = 1;
        current_mode = menu.selection();
        onModeEntry();
    } else if (BUTTON_ENTER_RELEASED && enter_old_state == 1) {
        enter_old_state = 0;
    }
}

void onModeEntry(void) {
    // reset the mode stage to 0
    mode_stage = 0;
    // sets up modes on startup and draws display
    switch(current_mode) {
        case MODE_PC:
            usbModeInit();
            twiPause();
            twiResume();
            display.clear();
            display.printFixed(0, 0, "PC Mode", STYLE_NORMAL);
            display.printFixed(0, 16, "Connect with the", STYLE_NORMAL);
            display.printFixed(0, 24, "KFDTool app.", STYLE_NORMAL);
            display.printFixed(0, 38, "Press enter to exit", STYLE_NORMAL);
            break;
        case MODE_VIEW_INFO:
            display.clear();

            // print the firmware rev
            
            #if defined(BETA)
            sprintf(buf, "FW V%d.%d.%d BETA", VER_FW_MAJOR, VER_FW_MINOR, VER_FW_PATCH);
            #else
            sprintf(buf, "FW V%d.%d.%d", VER_FW_MAJOR, VER_FW_MINOR, VER_FW_PATCH);
            #endif
            display.printFixed(0, 8, buf, STYLE_NORMAL);

            // print the adapter protocol
            sprintf(buf, "Proto V%d.%d.%d", VER_AP_MAJOR, VER_AP_MINOR, VER_AP_PATCH);
            display.printFixed(0, 16, buf, STYLE_NORMAL);

            // print the UID
            uint8_t ser0;
            uint8_t ser1;
            uint8_t ser2;
            uint8_t ser3;
            uint8_t ser4;
            uint8_t ser5;
            uint8_t ser6;
            uint8_t ser7;

            getUID8(&ser0, &ser1, &ser2, &ser3, &ser4, &ser5, &ser6, &ser7);

            sprintf(buf, "UID %d%d%d%d%d%d%d%d", ser0, ser1, ser2, ser3, ser4, ser5, ser6, ser7);
            display.printFixed(0, 24, buf, STYLE_NORMAL);

            // print the serial number
            uint16_t status;

            status = idReadSerNum(&ser0, &ser1, &ser2, &ser3, &ser4, &ser5);

            char sn[6];
            sn[0] = char(ser0);
            sn[1] = char(ser1);
            sn[2] = char(ser2);
            sn[3] = char(ser3);
            sn[4] = char(ser4);
            sn[5] = char(ser5);

            if (status == 1) {
                sprintf(buf, "SN %s", sn);
            } else {
                // serial is not set
                sprintf(buf, "SN NOT SET");
            }
            display.printFixed(0, 32, buf, STYLE_NORMAL);

            display.printFixed(0, 40, "", STYLE_NORMAL);
            display.printFixed(0, 52, "Enter to exit", STYLE_NORMAL);

            break;
        case MODE_KEY_ERASE:
            display.clear();
            display.printFixed(18, 0, "!!! WARNING !!!", STYLE_BOLD);
            display.printFixed(5, 8, "This will erase all", STYLE_NORMAL);
            display.printFixed(8, 16, "keys on the radio.", STYLE_NORMAL);
            display.printFixed(0, 32, "Enter to erase...", STYLE_NORMAL);
            display.printFixed(0, 40, "Reset to exit...", STYLE_NORMAL);
            twiPause();
            twiResume();
            break;

        case MODE_KEY_FILL:
            display.clear();
            
            // loop through all possible keys in eeprom
            uint8_t availableKeys[MAX_KEYS];
            uint8_t keyCount = 0;
            
            for (int i=0; i<MAX_KEYS; i++) {
                // we don't really care what happens to the data
                //TODO: make cute function to return valid keys rather than having to trash the data
                uint8_t flags;
                uint16_t ckr;
                uint16_t kid;
                uint8_t aid;
                uint8_t key[32];
                if (idReadKey(i, &flags, &ckr, &kid, &aid, key) == 1) {
                    availableKeys[keyCount] = i;
                    keyCount++;
                }
            }

            if (keyCount > 0) {
                const char *keyItems[keyCount];
                for (int i=0; i<keyCount; i++) {
                    // pull info for the key
                    uint8_t flags;
                    uint16_t ckr;
                    uint16_t kid;
                    uint8_t aid;
                    uint8_t key[32];
                    idReadKey(i, &flags, &ckr, &kid, &aid, key);

                    char * bufptr = (char*) malloc(20);
                    sprintf(bufptr, "%d: K %d A %d", availableKeys[i], (uint16_t) ckr, aid);
                    keyItems[i] = bufptr;
                }
                LcdGfxMenu keyMenu(keyItems, sizeof(keyItems) / sizeof(char *));
                keyMenu.show(display);
            } else {
                display.printFixed(18, 16, "No keys found", STYLE_BOLD);
            }

            break;

        case MODE_ZEROIZE:
            display.printFixed(18, 16, "Zeroizing KFD...", STYLE_BOLD);
            if (idZeroizeKeys() == 1) {
                display.printFixed(18, 32, "Zeroize success!", STYLE_NORMAL);
            } else {
                display.printFixed(18, 32, "Zeroize fail!", STYLE_BOLD);
            }
            delay(2000);
            current_mode = MODE_MAIN_MENU;
            break;
        case MODE_MAIN_MENU:
            display.clear();
            menu.show(display);
            break;

        default:
            break;
    }
}

void onModeExit(void) {
    // cleans up any modes upon exit
    switch(current_mode) {
        case 0:
            //usbModeCleanup();
            break;
        default:
            display.clear();
            break;
    }
}


#endif