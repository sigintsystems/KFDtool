#ifndef __CRC16_H__
#define __CRC16_H__

#include <Arduino.h>

uint16_t calculateFrameCrc16(uint8_t *val, uint8_t inLen);

#endif