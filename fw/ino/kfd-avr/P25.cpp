#include "P25.h"
#include "TwiProtocol.h"
#include "hal.h"
#include "HMI.h"

uint8_t P25_RXByte;
uint8_t P25_RXReady;
uint8_t waitingForResponse;

uint8_t OPCODE_READY_REQ = 0xC0;
uint8_t OPCODE_READY_GENERAL_MODE = 0xD0;
uint8_t OPCODE_TRANSFER_DONE = 0xC1;
uint8_t OPCODE_KMM = 0xC2;
uint8_t OPCODE_DISCONNECT_ACK = 0x90;
uint8_t OPCODE_DISCONNECT = 0x92;

uint8_t P25_MID_ZEROIZE_ALL = 0x21;

uint8_t P25_RESP_IMMEDIATE = 0x02;


bool P25KeyloadMr()
{
    //
}

bool P25ZeroizeKey(uint16_t ckr)
{
    //
}

bool P25ZeroizeKeys()
{
    twiSendKeySig();
    halDelayMs(10);
    twiSendPhyByte(OPCODE_READY_REQ);

    P25_RXByte = P25WaitUntilRXByte();
    
    
    if (P25_RXByte == OPCODE_READY_GENERAL_MODE)
    {
        hmiWriteAtLocation(0, 16, "Sending command");
        // 22-00-07-00-FF-FF-FF-00-00-01 - zeroize all
        int len = 9;
        byte frame[len];
        frame[0] = 0x22;
        frame[1] = 0x00;
        frame[2] = 0x07;
        frame[3] = 0x00;

        /* destination rsi */
        frame[4] = 0xFF;
        frame[5] = 0xFF;
        frame[6] = 0xFF;
        /* source rsi */
        frame[7] = 0x00;
        frame[8] = 0x00;
        frame[9] = 0x01;

        for (int i=0; i<=len; i++)
        {
            twiSendPhyByte(frame[i]);
        }

    }
    else
    {
        hmiWriteAtLocation(0, 16, "Command failed");
    }
    
}

uint8_t P25WaitUntilRXByte()
{
    bool done = false;
    uint8_t tmpRX;
    while (!done)
    {
        P25_RXReady = twiReceiveByte(&tmpRX);
        if (P25_RXReady == 1)
        {
            done = true;
            return tmpRX;
        }
    }
}