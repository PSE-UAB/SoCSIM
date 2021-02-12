/*!
 \file SoC.cpp
 \brief SoC
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later
#include <cstdio>
#include "SoC.h"
#include "Memory.h"
#include "HAL.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/** Timer clock frequency (16 MHz) */
#define TIMER_IN_FREQ (16000000)

SemaphoreHandle_t GUI_GPIO_IRQ;
TaskHandle_t GPIO_IRQ_handle;

#ifdef __cplusplus
extern "C" {
#endif

extern void PORT_A_ISR(void);
extern void PORT_B_ISR(void);

#ifdef __cplusplus
}
#endif

void GPIO_IRQ_thread(void *parameters) {

    while (true) {

        if (xSemaphoreTake(GUI_GPIO_IRQ, portMAX_DELAY) == pdTRUE) {
            uint32_t pending_irq = memory[ADDR_NVIC_IRQ];

            if (pending_irq & NVIC_PORTA_IRQ_BIT) {
                PORT_A_ISR();
            }

            if (pending_irq & NVIC_PORTB_IRQ_BIT) {
                PORT_B_ISR();
            }
        }
    }
}

void SoC_Init() {

    xTaskCreate(GPIO_IRQ_thread, "IRQ1", 10000, NULL, 1, &GPIO_IRQ_handle);

    GUI_GPIO_IRQ = xSemaphoreCreateBinary();
}

void I2CSlaveSet(int dev, int val) {

}


int PWMDutyGet() {
    int duty;

    if (memory[ADDR_TIMER_TOP] != 0) {
        duty = 100 * memory[ADDR_TIMER_CMP] / memory[ADDR_TIMER_TOP];
    }else{
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
    int freq;

    prescaler = TIMER_PrescalerGet();
    freq = TIMER_IN_FREQ / prescaler;
    return freq;
}
