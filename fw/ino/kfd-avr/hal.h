#ifndef HAL_H_
#define HAL_H_

#include <Arduino.h>
#include "Defines.h"

void halInit(void);

void halDelayUs(uint16_t us);

void halDelayMs(uint16_t ms);

void halEnterBsl(void);

void halReset(void);

void halActLedOn(void);

void halActLedOff(void);

void halActLedToggle(void);

void halSnsLedOn(void);

void halSnsLedOff(void);

void halSnsLedToggle(void);

void halGpio1High(void);

void halGpio1Low(void);

void halGpio1Toggle(void);

void halGpio2High(void);

void halGpio2Low(void);

void halGpio2Toggle(void);

void halKfdTxBusy(void);

void halKfdTxIdle(void);

void halSenTxConn(void);

void halSenTxDisc(void);

#endif /* HAL_H_ */