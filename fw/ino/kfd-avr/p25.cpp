#include "p25.h"
#include "TwiProtocol.h"
#include "crc16.h"

#include <Arduino.h>

uint8_t rxByte;
uint8_t frameTemp[128];

uint8_t keyload(uint16_t ckr, uint16_t sln, uint8_t aid, uint8_t *key) {
    //
}

uint16_t viewKeyset(void);

uint8_t activateKeyset(uint16_t keyset);

uint8_t viewKeys(void);

/*
Key Erasure
Erases all keys on target radio.

Return codes:
0 - Success
1 - Couldn't open KFD session
2 - no response after sending KMM

*/
uint8_t p25EraseAllKeys(void) {
    twiPause();
    twiResume();
    uint8_t error = 0;
    rxByte = 0;
    twiSendKeySig();
    delay(5);

    twiSendPhyByte(KFD_HELLO);
    rxWait(&rxByte);
    if (rxByte == MR_HELLO) {
        // the radio is here! let's continue...

        uint8_t frameBody[0];
        frameBody[0] = 0x00; // data

        uint8_t frameOutLen = buildKmmFrame(frameBody, 1, frameTemp, KMM_CMD_ZEROIZE);

        for (int i=0; i<frameOutLen; i++) {
            twiSendPhyByte(frameTemp[i]);
        }
        delay(5);

        //rxKmmFrame(frameTemp);

        //twiSendPhyByte(KFD_DISCONNECT);
        if (rxWait(&rxByte) == 1) {
            if (rxByte == DISCONNECT_ACK) {
                return 0;
            } else {
                error = rxByte;
            }
        } else {
            error = 2;
        }
    } else {
        error = 1;
    }

    return error;
}

uint8_t rxWait(uint8_t *c) {
    unsigned long start_millis;
    bool hasData = false;
    start_millis = millis();
    while (!hasData) {
        if (twiReceiveByte(c) == 1) {
            return 1;

        }
        if (millis() == start_millis + TIMEOUT) {
            return 0;
        }
    }
}

// length of the kmm body as seen by p25 spec
// not just the length of the array
uint8_t buildKmmFrame(uint8_t *inFrame, uint8_t inLen, uint8_t *outFrame, uint8_t commandOpcode) {
    uint8_t frameLen = inLen + 7 + 9; // 7 for inner body padding, 9 for outer body padding
    uint8_t totalLen = frameLen + 3; // 1 byte for header, 2 for crc

    outFrame[0] = KMM_TRANSFER;
    outFrame[1] = 0x00; // length high
    outFrame[2] = frameLen; // length low
    outFrame[3] = 0x00; // control opcode
    outFrame[4] = 0xFF; // dest RSI
    outFrame[5] = 0xFF; // dest RSI
    outFrame[6] = 0xFF; // dest RSI
    outFrame[7] = commandOpcode; // message ID / KMM opcode
    outFrame[8] = 0x00; // inner len - 7 + body length
    outFrame[9] = inLen + 7; // inner len
    outFrame[10] = 0x80; // response type
    outFrame[11] = 0xFF; // dest rsi
    outFrame[12] = 0xFF; // dest rsi
    outFrame[13] = 0xFF; // dest rsi
    outFrame[14] = 0xFF; // src rsi
    outFrame[15] = 0xFF; // src rsi
    outFrame[16] = 0xFF; // src rsi

    for (int i=0; i<inLen; i++) {
        outFrame[i+17] = inFrame[i];
    }

    uint16_t crc = calculateFrameCrc16(outFrame, frameLen - 2); // don't CRC the CRC lol

    outFrame[totalLen - 2] = crc & 0xFF; //crc high
    outFrame[totalLen - 1] = crc >> 8; //crc low
    return totalLen;
}

uint8_t rxKmmFrame(uint8_t *outFrame) {
    uint8_t frameIn [128];
    uint8_t tempByte;
    // receive first 3 bytes to determine length
    for (int i=0; i<4; i++) {
        rxWait(&tempByte);
    }
}