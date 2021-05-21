/*!
 \file HAL.h
 \brief Virtual HAL for simulator. It includes GPIO, NVIC, etc.
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


#ifndef _HAL_H_
#define _HAL_H_

#include "FreeRTOS.h"
#include "semphr.h"

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdbool.h>
#endif

/**
 * @brief GPIO available ports
 */
typedef enum {
    PORTA,
    PORTB,
    PORTC,
    PORTD
} Port;

/**
 * @brief Available ADC channeñs
 */
typedef enum {
    ADC_CHAN_0 = 0,
    ADC_CHAN_1 = 1,
} ADC_channel_t;

/**
 * @brief ADC run mode
 */
typedef enum {
    SINGLE = 0,
    DIFFERENTIAL = 1,
} ADC_mode_t;

/**
 * @brief Timer pre-scaler possible values
 */
typedef enum {
    PRESCALER1 = 1,
    PRESCALER2 = 2,
    PRESCALER4 = 4,
    PRESCALER8 = 8,
    PRESCALER16 = 16,
    PRESCALER32 = 32,
    PRESCALER64 = 64,
    PRESCALER128 = 128,
    PRESCALER256 = 256,
} timer_prescaler_t;

/**
 * @brief Watchdog pre-scaler possible values
 */
typedef enum {
    WDT_16_MS = 0,
    WDT_32_MS,
    WDT_64_MS,
    WDT_125_MS,
    WDT_250_MS,
    WDT_500_MS,
    WDT_1000_MS,
    WDT_2000_MS,
    WDT_4000_MS,
    WDT_8000_MS,
} wdt_cycles_t;

/************************************ GPIO ***********************************/

/**
 * @brief Configures pin direction
 * @param port GPIO Port
 * @param pin Pin
 * @param out True: the pin is an Output, False: the pin is an input
 * @return true on success
 */
bool GPIO_PinCfg(Port port, uint32_t pin, bool out);

/**
 * @brief Sets '1' value in output pin (if properly configured)
 * @param port GPIO Port
 * @param pin Pin
 * @return true on success
 */
bool GPIO_PinSet(Port port, uint32_t pin);

/**
 * @brief Sets '0' value in output pin (if properly configured)
 * @param port GPIO Port
 * @param pin Pin
 * @return true on success
 */
bool GPIO_PinClear(Port port, uint32_t pin);

/**
 * @brief Toggles output value in pin (if properly configured)
 * @param port GPIO Port
 * @param pin Pin
 * @return true on success
 */
bool GPIO_PinToggle(Port port, uint32_t pin);

/**
 * @brief Enables interrupt generation for selected pin
 * @param port GPIO Port
 * @param pin Pin
 * @return true on success
 */
bool GPIO_IntEnable(Port port, uint32_t pin);

/**
 * @brief Disables interrupt generation for selected pin
 * @param port GPIO Port
 * @param pin Pin
 * @return true on success
 */
bool GPIO_IntDisable(Port port, uint32_t pin);


/************************************ NVIC ***********************************/
/**
 * @brief Enables IRQ
 * @param irq IRQ to enable
 * @return true on success
 */
bool NVIC_Enable(uint32_t irq);

/**
 * @brief Disables IRQ
 * @param irq IRQ to disable
 * @return true on success
 */
bool NVIC_Disable(uint32_t irq);

/**
 * @brief Clears pending IRQ
 * @param irq IRQ to clear
 * @return true on success
 */
bool NVIC_IntClear(uint32_t irq);

/************************************ Trace ***********************************/

/**
 * @brief Outputs character by the debug port
 * @param c character to print
 * @return true on success
 */
bool HAL_Trace(char c);


/************************************ PWM Timer *******************************/
/**
 * @brief Sets pre-scaler for TIMER
 * @param pres divider selection
 * @return true on success
 */
bool TIMER_PrescalerSet(timer_prescaler_t pres);

/**
 * @brief Gets divider value for TIMER
 * @return Divider value
 */
timer_prescaler_t TIMER_PrescalerGet();

/**
 * @brief Sets TOP value for TIMER
 * @param top TOP value
 * @return true on success
 */
bool TIMER_SetTOP(uint16_t top);

/**
 * @brief Sets compare value for TIMER
 * @param cmp compare value
 * @return true on success
 */
bool TIMER_SetCMP(uint16_t cmp);

/**
 * @brief Enables TIMER
 * @return true on success
 */
bool TIMER_Enable();

/**
 * @brief Disables TIMER
 * @return true on success
 */
bool TIMER_Disable();

/************************************ I2C  ***********************************/


/************************************ RTC  ***********************************/
/**
 * @brief Enables RTC
 * @return true on success
 */
bool RTC_Enable();

/**
 * @brief Disables RTC
 * @return true on success
 */
bool RTC_Disable();

/**
 * @brief Sets compare value for RTC
 * @param value unix epoch value to set
 * @return true on success
 */
bool RTC_CompareSet(uint32_t value);

/**
 * @brief Gets compare value for RTC
 * @return Unix epoch value
 */
uint32_t RTC_CompareGet();

/**
 * @brief Sets date/time to RTC
 * @param value date/time to set in unix epoch format
 * @return true on success
 */
bool RTC_CounterSet(uint32_t value);

/**
 * @brief Gets date/time from RTC
 * @return value date/time in unix epoch format
 */
uint32_t RTC_CounterGet();

/**
 * @brief Enables IRQ generation for RTC
 * @return true on success
 */
bool RTC_IntEnable();

/**
 * @brief Disables IRQ generation
 * @return true on success
 */
bool RTC_IntDisable();

/************************************ DAC  ***********************************/
/**
 * @brief Enable DAC peripheral
 * @return true on success
 */
bool DAC_Enable();

/**
 * @brief Disable DAC peripheral
 * @return true on success
 */
bool DAC_Disable();

/**
 * @brief Enable DAC interrupts
 * @return true on success
 */
bool DAC_IntEnable();

/**
 * @brief Disable DAC interrupts
 * @return true on success
 */
bool DAC_IntDisable();

/**
 * @brief Set DAC output value
 * @param value value to set
 * @return true on success
 */
bool DAC_Set(uint16_t value);

/************************************ UART ***********************************/
/**
 * @brief Enables UART peripheral
 * @return true on success
 */
bool UART_Enable();

/**
 * @brief Disables UART peripheral
 * @return true on success
 */
bool UART_Disable();

/**
 * @brief Enables UART interrupts
 * @return true on success
 */
bool UART_IntEnable();

/**
 * @brief Disables UART interrupts
 * @return true on success
 */
bool UART_IntDisable();

/**
 * @brief Returns UART status register
 * @return status register value
 */
uint32_t UART_GetStatus();

/**
 * @brief Receive one frame
 * @return Data received
 */
uint8_t UART_Rx();

/**
 * @brief Transmit one frame
 * @param data Data to transmit
 * @return true on success
 */
bool UART_Tx(uint8_t data);

/************************************ ADC ************************************/

/**
 * @brief initializes ADC and sets proper mode
 * @param mode ADC can be configured as single-ended or differential
 * @return true
 */
bool ADC_Init(ADC_mode_t mode);

/**
 * @brief Enables ADC peripheral
 * @return true
 */
bool ADC_Enable();

/**
 * @brief Disables ADC peripheral
 * @return true
 */
bool ADC_Disable();

/**
 * @brief Enables ADC interrupts
 * @return true
 */
bool ADC_IntEnable();

/**
 * @brief Disables ADC interrupts
 * @return true
 */
bool ADC_IntDisable();

/**
 * @brief Enables ADC conversion for channel
 * @param ch channel to convert (in single-ended mode)
 * @return true
 */
bool ADC_EnableChannel(ADC_channel_t ch);

/**
 * @brief Get ADC status
 * @return ADC_STATUS register value
 */
uint32_t ADC_GetStatus();

/**
 * @brief returns last ADC conversion data
 * @return conversion data
 */
uint32_t ADC_GetData();



/************************************ WDT ************************************/

/**
 * @brief Enables system watchdog
 * @return true
 */
bool WDOG_Enable();

/**
 * @brief Sets watchdog prescaler
 * @param pres Prescaler value
 * @return true
 */
bool WDOG_PrescalerSet(wdt_cycles_t pres);

/**
 * @brief Resets the watchdog to avoid system reset
 * @return true
 */
bool WDOG_Feed();

/******************************** Memory access ******************************/

/**
 * @brief Debug function to write to a memory address
 * @param addr address to access
 * @param data data to write
 */
void HAL_MemoryWrite(uint32_t addr, uint32_t data);

/**
 * @brief Debug function to read from a memory address
 * @param addr address to access
 * @return data read
 */
uint32_t HAL_MemoryRead(uint32_t addr);

#ifdef __cplusplus
}
#endif

#endif