/*!
 \file HAL.h
 \brief Virtual HAL for simulator. It includes GPIO, NVIC, etc.
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later

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

/************************************ GPIO ***********************************/
bool GPIO_PinCfg(Port port, int pin, bool out);
bool GPIO_PinSet(Port port, int pin);
bool GPIO_PinClear(Port port, int pin);
bool GPIO_PinToggle(Port port, int pin);
bool GPIO_IntEnable(Port port, int pin);
bool GPIO_IntDisable(Port port, int pin);


/************************************ NVIC ***********************************/
bool NVIC_Enable(int irq);
bool NVIC_Disable(int irq);
bool NVIC_IntClear(int irq);

/************************************ Trace ***********************************/
bool HAL_Trace(char c);


/************************************ PWM Timer *******************************/
bool TIMER_PrescalerSet(int pres);
int TIMER_PrescalerGet();
bool TIMER_SetTOP(int top);
bool TIMER_SetCMP(int cmp);
bool TIMER_Enable();
bool TIMER_Disable();

/************************************ I2C  ***********************************/


#ifdef __cplusplus
}
#endif
