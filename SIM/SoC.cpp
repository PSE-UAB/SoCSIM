/*!
 \file SoC.cpp
 \brief SoC
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later
#include <cstdio>
#include <time.h>

#include "SoC.h"
#include "Memory.h"
#include "HAL.h"
#include "GUI.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/** Timer clock frequency (16 MHz) */
#define TIMER_IN_FREQ (16000000)

/**
 * @brief BIT for PORT A IRQ in the NVIC register
 */
#define NVIC_PORTA_IRQ_BIT (1 << NVIC_PORTA_IRQ_NUM)

/**
 * @brief BIT for PORT B IRQ in the NVIC register
 */
#define NVIC_PORTB_IRQ_BIT (1 << NVIC_PORTB_IRQ_NUM)

/**
 * @brief BIT for PORT C IRQ in the NVIC register
 */
#define NVIC_PORTC_IRQ_BIT (1 << NVIC_PORTC_IRQ_NUM)

/**
 * @brief BIT for PORT D IRQ in the NVIC register
 */
#define NVIC_PORTD_IRQ_BIT (1 << NVIC_PORTD_IRQ_NUM)



/**
 * @brief BIT for RTC IRQ in the NVIC register
 */
#define NVIC_RTC_IRQ_BIT (1 << NVIC_RTC_IRQ_NUM)
/**
 * @brief Semaphore to indicate that a GPIO IRQ is triggered
 */
SemaphoreHandle_t GUI_GPIO_IRQ;

/**
 * @brief Semaphore to indicate that RTC has triggered its IRQ
 */
SemaphoreHandle_t RTC_IRQ;

/**
 * @brief GPIO IRQ task, it depends on #GUI_GPIO_IRQ
 */
TaskHandle_t GPIO_IRQ_handle;

/**
 * @brief RTC IRQ task
 */
TaskHandle_t RTC_IRQ_handle;

/* forward declaration */
void RTC_IRQ_thread(void*);

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief PORT A ISR must be defined by the user
 */
__attribute__((weak)) void PORT_A_ISR(void);

/**
 * @brief PORT B ISR must be defined by the user
 */
__attribute__((weak)) void PORT_B_ISR(void);

/**
 * @brief RTC ISR must be defined by the user
 */
__attribute__((weak)) void RTC_ISR(void);

#ifdef __cplusplus
}
#endif

/**
 * @brief Thread to manage GPIO IRQs
 * @param parameters unused
 */
void GPIO_IRQ_thread(void *parameters) {

    while (true) {

        if (xSemaphoreTake(GUI_GPIO_IRQ, portMAX_DELAY) == pdTRUE) {
            uint32_t pending_irq = memory[ADDR_NVIC_IRQ];

            if (pending_irq & NVIC_PORTA_IRQ_BIT) {
                PORT_A_ISR();
                xSemaphoreGive(GUI_GPIO_IRQ);
                vTaskDelay(10);
            }

            if (pending_irq & NVIC_PORTB_IRQ_BIT) {
                PORT_B_ISR();
                xSemaphoreGive(GUI_GPIO_IRQ);
                vTaskDelay(10);
            }
        }
    }
}

void GPIO_in_cb(int val, int param) {

    uint32_t addr;
    uint32_t bit;

    switch (param) {
    case 1:
        addr = ADDR_PORTA_INT;
        bit = NVIC_PORTA_IRQ_BIT;
        break;
    case 2:
        addr = ADDR_PORTB_INT;
        bit = NVIC_PORTB_IRQ_BIT;
        break;
    case 3:
        addr = ADDR_PORTC_INT;
        bit = NVIC_PORTC_IRQ_BIT;
        break;
    case 4:
        addr = ADDR_PORTD_INT;
        bit = NVIC_PORTD_IRQ_BIT;
        break;
    default:
        addr = 0;
        bit = 0;
        break;
    }

    if ((memory[addr] & val) != 0) {
        uint32_t aux = memory[ADDR_NVIC_IRQ];
        aux |= bit;
        memory[ADDR_NVIC_IRQ] = aux;
        xSemaphoreGive(GUI_GPIO_IRQ);
    }
}

void Trace_cb(int val, int param) {
    gui_add_trace(val);
}

void SoC_Init() {

    xTaskCreate(GPIO_IRQ_thread, "IRQ1", 10000, NULL, 1, &GPIO_IRQ_handle);

    xTaskCreate(RTC_IRQ_thread, "RTC", 10000, NULL, 1, &RTC_IRQ_handle);

    memory[ADDR_PORTA_IN].register_wr_cb(GPIO_in_cb, 1);
    memory[ADDR_PORTB_IN].register_wr_cb(GPIO_in_cb, 2);
    memory[ADDR_PORTC_IN].register_wr_cb(GPIO_in_cb, 3);
    memory[ADDR_PORTD_IN].register_wr_cb(GPIO_in_cb, 4);

    memory[ADDR_TRACE].register_wr_cb(Trace_cb, 0);

    GUI_GPIO_IRQ = xSemaphoreCreateBinary();
    RTC_IRQ = xSemaphoreCreateBinary();
}

void I2CSlaveSet(int dev, int val) {

}

void SoC_Button1Pressed() {
#if 0
    if (memory[ADDR_PORTA_INT] & (1 << BUTTON_1_PIN)) {
        //memory[ADDR_NVIC_IRQ] |= NVIC_PORTA_IRQ_BIT;
        uint32_t aux = memory[ADDR_NVIC_IRQ];
        aux  |= NVIC_PORTA_IRQ_BIT;
        memory[ADDR_NVIC_IRQ] = aux;
        xSemaphoreGive(GUI_GPIO_IRQ);
    }
#else
    uint32_t aux = memory[ADDR_PORTA_IN];
    aux |= (1 << BUTTON_1_PIN);
    memory[ADDR_PORTA_IN] = aux;
#endif
}

void SoC_Button1Released() {
    uint32_t aux = memory[ADDR_PORTA_IN];
    aux &= ~(1 << BUTTON_1_PIN);
    memory[ADDR_PORTA_IN] = aux;
}

void SoC_Button2Pressed() {
#if 0
    if (memory[ADDR_PORTB_INT] & (1 << BUTTON_2_PIN)) {
        //memory[ADDR_NVIC_IRQ] |= NVIC_PORTB_IRQ_BIT;
        uint32_t aux = memory[ADDR_NVIC_IRQ];
        aux  |= NVIC_PORTB_IRQ_BIT;
        memory[ADDR_NVIC_IRQ] = aux;
        xSemaphoreGive(GUI_GPIO_IRQ);
    }
#else
    uint32_t aux = memory[ADDR_PORTB_IN];
    aux |= (1 << BUTTON_2_PIN);
    memory[ADDR_PORTB_IN] = aux;
#endif
}

void SoC_Button2Released() {
    uint32_t aux = memory[ADDR_PORTB_IN];
    aux &= ~(1 << BUTTON_2_PIN);
    memory[ADDR_PORTB_IN] = aux;
}

bool SoC_LED1On() {
    if (memory[ADDR_PORTC_CTRL] & (1 << LED_1_PIN)) {
        if (memory[ADDR_PORTC_OUT] & (1 << LED_1_PIN)) {
            return true;
        }
    }

    return false;
}

bool SoC_LED2On() {

    if (memory[ADDR_PORTD_CTRL] & (1 << LED_2_PIN)) {
        if (memory[ADDR_PORTD_OUT] & (1 << LED_2_PIN)) {
            return true;
        }
    }
    return false;
}

int PWMDutyGet() {
    int duty;

    if (memory[ADDR_TIMER_TOP] != 0) {
        duty = 100 * memory[ADDR_TIMER_CMP] / memory[ADDR_TIMER_TOP];
    } else {
        duty = 0;
    }

    return duty;
}

int PWMFreqGet() {
    int freq;

    if (memory[ADDR_TIMER_TOP] != 0) {
        freq = TimerFreqGet() / memory[ADDR_TIMER_TOP];
    } else {
        freq = 0;
    }

    return freq;
}

int TimerFreqGet() {
    int prescaler;
    int freq = 0;

    prescaler = TIMER_PrescalerGet();
    if (prescaler != 0) {
        freq = TIMER_IN_FREQ / prescaler;
    }
    return freq;
}

/******************** RTC *******************/

void RTC_IRQ_thread(void *parameters) {

    TickType_t pxPreviousWakeTime;

    while (true) {
        time_t now;
        time(&now);

        if (memory[ADDR_RTC_CTRL] & 0x01) {
            memory[ADDR_RTC_CNT] = now;
        }

        if (memory[ADDR_RTC_CTRL] & 0x00000080) {
            if (now == memory[ADDR_RTC_CMP]) {
                //memory[ADDR_NVIC_IRQ] |= NVIC_RTC_IRQ_BIT;
                uint32_t aux = memory[ADDR_NVIC_IRQ];
                aux |= NVIC_RTC_IRQ_BIT;
                memory[ADDR_NVIC_IRQ] = aux;
                RTC_ISR();
            }
        }

        /* Check every 1 s. */
        vTaskDelayUntil(&pxPreviousWakeTime, 1000);
    }
}

