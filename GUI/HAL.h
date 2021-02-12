/*!
 \file HAL.h
 \brief HAL
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


/**
 * Virtual HAL for simulator.
 * It includes GPIO, NVIC,
*/


#include <stdint.h>
#include "FreeRTOS.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

typedef enum {
    PORTA,
    PORTB,
    PORTC,
    PORTD
} Port;


/********************************** MCU Side *********************************/
/************************************ GPIO ***********************************/
bool GPIO_PinSet(Port port, int pin);
bool GPIO_PinClear(Port port, int pin);
bool GPIO_PinToggle(Port port, int pin);
bool GPIO_IntEnable(Port port, int pin);
bool GPIO_IntDisable(Port port, int pin);


/************************************ NVIC ***********************************/
bool NVIC_Enable(int irq);
bool NVIC_Disable(int irq);
bool NVIC_IntClear(int irq);

bool HAL_Trace(char c);

/************************************ I2C  ***********************************/


/********************************** GUI Side *********************************/
void I2CSlaveSet(int dev, int val);

#ifdef __cplusplus
}
#endif
