#ifndef INFODATA_H_
#define INFODATA_H_

#include "stdint.h"
#include <EEPROM.h>

uint16_t idWriteModelIdHwRev(uint8_t hwId, uint8_t hwRevMaj, uint8_t hwRevMin);

uint16_t idWriteSerNum(uint8_t ser0, uint8_t ser1, uint8_t ser2, uint8_t ser3, uint8_t ser4, uint8_t ser5);

uint16_t idReadModelId(uint8_t *hwId);

uint16_t idReadHwRev(uint8_t *hwRevMaj, uint8_t *hwRevMin);

uint16_t idReadSerNum(uint8_t *ser0, uint8_t *ser1, uint8_t *ser2, uint8_t *ser3, uint8_t *ser4, uint8_t *ser5);

uint16_t idReadKey(uint8_t slot, uint8_t *flags, uint16_t *ckr, uint16_t *kid, uint8_t *aid, uint8_t *key);

uint16_t idWriteKey(uint8_t slot, uint8_t flags, uint16_t ckr, uint16_t kid, uint8_t aid, uint8_t *key);

uint8_t idZeroizeKeys();

#endif /* INFODATA_H_ */
