/*!
 \file SoC.h
 \brief SoC
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


#ifndef GUI_SOC_H_
#define GUI_SOC_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BUTTON_1_PORT PORTA
#define BUTTON_1_PIN 0

#define BUTTON_2_PORT PORTB
#define BUTTON_2_PIN 1

#define LED_1_PORT PORTC
#define LED_1_PIN 7

#define LED_2_PORT PORTD
#define LED_2_PIN 12

#define NVIC_PORTA_IRQ_NUM 0
#define NVIC_PORTB_IRQ_NUM 1


#define NVIC_PORTA_IRQ_BIT (1 << NVIC_PORTA_IRQ_NUM)
#define NVIC_PORTB_IRQ_BIT (1 << NVIC_PORTB_IRQ_NUM)

#define TIMER_CTRL_PRESCALER_SHIFT (8)

extern SemaphoreHandle_t GUI_GPIO_IRQ;

void SoC_Init();


/********************************** GUI Side *********************************/
void I2CSlaveSet(int dev, int val);

void SoC_Button1Pressed();
void SoC_Button2Pressed();
bool SoC_LED1On();
bool SoC_LED2On();

int PWMDutyGet();
int PWMFreqGet();
int TimerFreqGet();


#ifdef __cplusplus
}
#endif

#endif /* GUI_SOC_H_ */
