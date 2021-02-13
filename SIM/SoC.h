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

/** BUTTON 1 is connected to Pin 0 */
#define BUTTON_1_PIN 0

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



/** Shif value to access PRESCALER value on TIMER_CTRL register */
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
 * @brief Button 2 is pressed. Updates all necessary GPIO registers
 */
void SoC_Button2Pressed();

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
int PWMDutyGet();

/**
 * @brief Calculates duty cycle frequency
 * @return PWM frequency
 */
int PWMFreqGet();

/**
 * @brief Calculates TIMER frequency
 * @return TIMER frequency
 */
int TimerFreqGet();


#ifdef __cplusplus
}
#endif

#endif /* GUI_SOC_H_ */
