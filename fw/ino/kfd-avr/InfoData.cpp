#include "InfoData.h"
#include "Defines.h"

#define INFOB_START 0x0
#define INFOB_LENGTH 5
#define INFOB_HEADER 0x10
#define INFOB_FOOTER 0x11

#define INFOC_START 0x10
#define INFOC_LENGTH 8
#define INFOC_HEADER 0x20
#define INFOC_FOOTER 0x22

#define KEY_START 0x50
#define KEY_LENGTH 0x40
#define KEY_HEADER 0x60
#define KEY_FOOTER 0x61

// the key eeprom layout is flags,ckr,kid,aid,key
// not all keys will use the full 40 bytes of space but it pads it enough for aes
// multikey with support for ~15 total keys

uint16_t idWriteModelIdHwRev(uint8_t hwId, uint8_t hwRevMaj, uint8_t hwRevMin)
{

    uint8_t data[INFOB_LENGTH];
    data[0] = INFOB_HEADER;
    data[1] = hwId;
    data[2] = hwRevMaj;
    data[3] = hwRevMin;
    data[4] = INFOB_FOOTER;
    
    for (int i=0; i<INFOB_LENGTH; i++)
    {
        // use update instead of write to possibly save eeprom cells from unnecessary writes
        EEPROM.update(INFOB_START+i, data[i]);
    }
    return 1;
}

uint16_t idWriteSerNum(uint8_t ser0, uint8_t ser1, uint8_t ser2, uint8_t ser3, uint8_t ser4, uint8_t ser5)
{
    uint8_t data[INFOC_LENGTH];
    data[0] = INFOC_HEADER;
    data[1] = ser0;
    data[2] = ser1;
    data[3] = ser2;
    data[4] = ser3;
    data[5] = ser4;
    data[6] = ser5;
    data[7] = INFOC_FOOTER;
    
    for (int i=0; i<INFOC_LENGTH; i++)
    {
        // use update instead of write to possibly save eeprom cells from unnecessary writes
        EEPROM.update(INFOC_START+i, data[i]);
    }
    return 1;
}

/*
uint8_t slot - internal kfd-avr slot that the key sits in
uint8_t flags - staus flags for keys, currently 1 means it is XORed and requires a passcode
uint16_t ckr - p25 ckr
uint16_t kid - p25 kid
uint8_t aid - algid (0x84, 0x81, etc)
uint8_t key[] - array of key bytes
*/
uint16_t idWriteKey(uint8_t slot, uint8_t flags, uint16_t ckr, uint16_t kid, uint8_t aid, uint8_t *key)
{
    // error out if we are attmpting to write more keys than we have space for:
    if (slot > MAX_KEYS) {
        return 0;
    }
    uint8_t data[KEY_LENGTH];
    data[0] = KEY_HEADER;
    data[1] = flags;
    data[2] = ((ckr >> 8) & 0xFF);
    data[3] = (ckr & 0xFF);
    data[4] = ((kid >> 8) & 0xFF);
    data[5] = (kid & 0xFF);
    data[6] = aid;

    for (int i=0; i<32; i++) {
        data[7+i] = key[i];
    }
    

    data[KEY_LENGTH - 1] = KEY_FOOTER;
    
    for (int i=0; i<KEY_LENGTH; i++)
    {
        // use update instead of write to possibly save eeprom cells from unnecessary writes
        EEPROM.update(KEY_START + (KEY_LENGTH * slot) + i, data[i]);
    }
    return 1;
}

uint8_t idZeroizeKeys() {
    for (int i=0; i<KEY_LENGTH * MAX_KEYS; i++)
    {
        // force write all key locations to zero
        EEPROM.write(KEY_START + i, 0);
    }
    return 1;   
}

uint16_t idReadModelId(uint8_t *hwId)
{
    uint8_t data[INFOB_LENGTH];

    for (int i=0; i<INFOB_LENGTH; i++)
    {
        data[i] = EEPROM.read(INFOB_START + i);
    }
    
    uint8_t header;
    uint8_t footer;

    header = data[0];
    footer = data[INFOB_LENGTH - 1];
    *hwId = data[1];
    
    if (header == INFOB_HEADER && footer == INFOB_FOOTER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t idReadHwRev(uint8_t *hwRevMaj, uint8_t *hwRevMin)
{
    uint8_t data[INFOB_LENGTH];

    for (int i=0; i<INFOB_LENGTH; i++)
    {
        data[i] = EEPROM.read(INFOB_START + i);
    }
    
    uint8_t header;
    uint8_t footer;

    header = data[0];
    footer = data[INFOB_LENGTH - 1];
    *hwRevMaj = data[2];
    *hwRevMin = data[3];
    
    if (header == INFOB_HEADER && footer == INFOB_FOOTER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

uint16_t idReadSerNum(uint8_t *ser0, uint8_t *ser1, uint8_t *ser2, uint8_t *ser3, uint8_t *ser4, uint8_t *ser5)
{
    uint8_t data[INFOC_LENGTH];

    for (int i=0; i<INFOC_LENGTH; i++)
    {
        data[i] = EEPROM.read(INFOC_START + i);
    }
    
    uint8_t header;
    uint8_t footer;

    header = data[0];
    footer = data[INFOC_LENGTH - 1];
    *ser0 = data[1];
    *ser1 = data[2];
    *ser2 = data[3];
    *ser3 = data[4];
    *ser4 = data[5];
    *ser5 = data[6];
    
    if (header == INFOC_HEADER && footer == INFOC_FOOTER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
uint8_t slot - internal kfd-avr slot that the key sits in
uint16_t ckr - p25 ckr
uint16_t kid - p25 kid
uint8_t aid - algid (0x84, 0x81, etc)
uint8_t key[] - array of key bytes
*/
uint16_t idReadKey(uint8_t slot, uint8_t *flags, uint16_t *ckr, uint16_t *kid, uint8_t *aid, uint8_t *key)
{
    uint8_t data[KEY_LENGTH];

    for (int i=0; i<KEY_LENGTH; i++)
    {
        data[i] = EEPROM.read(KEY_START + (KEY_LENGTH * slot) + i);
    }
    
    uint8_t header;
    uint8_t footer;
    uint8_t ckrTemp1;
    uint8_t ckrTemp2;
    uint8_t kidTemp1;
    uint8_t kidTemp2;

    ckrTemp1 |= data[2] << 8;
    ckrTemp1 |= data[3];
    kidTemp1 |= data[4] << 8;
    kidTemp1 |= data[5];

    header = data[0];
    *flags = data[1];
    *ckr = ckrTemp1;
    *kid = kidTemp1;
    *aid = data[6];

    footer = data[KEY_LENGTH - 1];

    for (int i=0; i<32; i++) {
        key[i] = data[7+i];
    }
    
    if (header == KEY_HEADER && footer == KEY_FOOTER)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}