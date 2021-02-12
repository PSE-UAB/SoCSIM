/*!
 \file BSP.cpp
 \brief BSP
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later
#include <cstdio>
#include "BSP.h"

#include "Memory.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

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

void BSP_Init() {

    xTaskCreate(GPIO_IRQ_thread, "IRQ1", 10000, NULL, 1, &GPIO_IRQ_handle);

    GUI_GPIO_IRQ = xSemaphoreCreateBinary();
}

