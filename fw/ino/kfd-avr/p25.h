#ifndef P25_H_
#define P25_H_

#include <Arduino.h>

#define TIMEOUT 3000

#define KFD_HELLO 0xC0
#define MR_HELLO 0xD0
#define KMM_TRANSFER 0xC2
#define KMM_TRANSFER_DONE 0xC1
#define KFD_DISCONNECT 0x92
#define DISCONNECT_ACK 0x90

#define KMM_CMD_MODIFY_KEY 0x13

#define KMM_CMD_INVENTORY 0x0D
#define KMM_RESP_INVENTORY 0x0E
#define KMM_INVENTORY_LIST_ACTIVE_KEYSETS 0x02
#define KMM_INVENTORY_LIST_ACTIVE_KEYS 0xFD

#define KMM_CMD_ZEROIZE 0x21
#define KMM_RESP_ZEROIZE 0x22

struct  KmmZeroizeCommand{
   uint8_t opcode = KMM_TRANSFER;
   uint16_t len;
   uint8_t crc1;
   uint8_t crc2;
};

uint8_t keyload(uint16_t ckr, uint16_t sln);

uint16_t viewKeyset(void);

uint8_t activateKeyset(uint16_t keyset);

uint8_t viewKeys(void);

uint8_t p25EraseAllKeys(void);

uint8_t rxWait(uint8_t *c);

uint8_t buildKmmFrame(uint8_t *inFrame, uint8_t inLen, uint8_t *outFrame, uint8_t length);

uint8_t rxKmmFrame(uint8_t *outFrame);

#endif