#ifndef P25_H_
#define P25_H_
#include "stdint.h"

bool P25KeyloadMr();

bool P25ZeroizeKey(uint16_t ckr);

bool P25ZeroizeKeys();

uint8_t P25WaitUntilRXByte();

#endif