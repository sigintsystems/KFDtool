#include "HMI.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
uint8_t oldMode = 0; // temp for storing the old mode (to see if we changed modes recently)
uint8_t P25OutData[128];

void hmiUpdateMode(uint8_t mode)
{
    if (oldMode != mode)
    {
        hmiUpdateMode(mode);
        oldMode = mode;
    }
}
void hmiInitDisplay()
{
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.clearDisplay();
    display.display();
}
void hmiClearDisplay(){
    display.clearDisplay();
    display.display();
}

void hmiWriteAtLocation(uint8_t x, uint8_t y, const char * text)
{
    display.setCursor(x, y);
    display.print(text);
    display.display();
}

void hmiRedrawMenus(uint8_t mode){
    // caled once to redraw menus

    // set up the display
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);

    if (mode == 0)
    {
        display.println("Main Menu");
    }
    else if (mode == 1) // serial mode
    {
        display.setTextColor(BLACK, WHITE);
        display.println("    Serial Mode      ");
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 16);
        display.println("Waiting for host...");
    }
    else if (mode == 2) // auto zeroize mode
    {
        display.setTextColor(BLACK, WHITE);
        display.println("    Auto-Zeroize    ");
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 8);
        display.println("Connect radio now...");
    }
    display.display();
}

void hmiDoSplash()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.cp437(true);
    display.println("KFD-AVR");
    display.println("V1.6");
    display.display();
}