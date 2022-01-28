#include "hal.h"
#include "TwiProtocol.h"
#include "InfoData.h"
#include "ControlOpCodes.h"
#include "Versions.h"
#include "UID.h"
#include "Display.h"
#include "modes/usb.h"


void setup()
{
    halInit();
    displayInit();
    twiInit();
}

void loop()
{
    #if defined(DISPLAY_ENABLED)
    displayLoop();
    #else
    usbModeLoop();
    #endif
}
