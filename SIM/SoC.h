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

/** BUTTON 1 is connected to Port A */
#define BUTTON_1_PORT PORTA

/** BUTTON 1 is connected to Pin 11 */
#define BUTTON_1_PIN 11

/** BUTTON 2 is connected to Port B */
#define BUTTON_2_PORT PORTB

/** BUTTON 2 is connected to Pin 1 */
#define BUTTON_2_PIN 1

/** LED 1 is connected to Port C */
#define LED_1_PORT PORTC

/** LED 1 is connected to Pin 7 */
#define LED_1_PIN 7

/** LED 1 is connected to Port D */
#define LED_2_PORT PORTD

/** LED 2 is connected to Pin 12 */
#define LED_2_PIN 12

/** PORT A has IRQ #0 */
#define NVIC_PORTA_IRQ_NUM 0

/** PORT B has IRQ #1 */
#define NVIC_PORTB_IRQ_NUM 1

/** PORT C has IRQ #2 */
#define NVIC_PORTC_IRQ_NUM 2

/** PORT D has IRQ #3 */
#define NVIC_PORTD_IRQ_NUM 3

/** RTC has IRQ #14 */
#define NVIC_RTC_IRQ_NUM 14

/** DAC has IRQ #16 */
#define NVIC_DAC_IRQ_NUM 16

/** UART has irq #23 */
#define NVIC_UART_IRQ_NUM 23

/** Shift value to access PRESCALER value on TIMER_CTRL register */
#define TIMER_CTRL_PRESCALER_SHIFT (8)

extern SemaphoreHandle_t GUI_GPIO_IRQ;

/**
 * @brief Initializes SoC library
 */
void SoC_Init();


/********************************** GUI Side *********************************/
/**
 * TBD
 * @param dev
 * @param val
 */
void I2CSlaveSet(int dev, int val);

/**
 * @brief Button 1 is pressed. Updates all necessary GPIO registers
 */
void SoC_Button1Pressed();

/**
 * @brief Button 1 is released. Updates all necessary GPIO registers
 */
void SoC_Button1Released();

/**
 * @brief Button 2 is pressed. Updates all necessary GPIO registers
 */
void SoC_Button2Pressed();

/**
 * @brief Button 2 is released. Updates all necessary GPIO registers
 */
void SoC_Button2Released();

/**
 * @brief Checks if LED 1 should be on.
 * @return true if LED should be on
 */
bool SoC_LED1On();

/**
 * @brief Checks if LED 2 should be on
 * @return true if LED should be on
 */
bool SoC_LED2On();

/**
 * @brief Calculates duty cycle for TIMER
 * @return PWM Duty cycle
 */
unsigned int PWMDutyGet();

/**
 * @brief Calculates duty cycle frequency
 * @return PWM frequency
 */
unsigned int PWMFreqGet();

/**
 * @brief Calculates TIMER frequency
 * @return TIMER frequency
 */
unsigned int TimerFreqGet();


const int DAC_TOTAL_VALUES = 50;
float get_DACVal (void* data, int idx);

uint16_t UART_GetBaudRate();

void UART_NotifyRxData();

#ifdef __cplusplus
}
#endif

const char *getUART_Path();

#endif /* GUI_SOC_H_ */
