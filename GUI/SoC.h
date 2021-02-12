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

#define NVIC_PORTA_IRQ_NUM 0
#define NVIC_PORTB_IRQ_NUM 1


#define NVIC_PORTA_IRQ_BIT (1 << NVIC_PORTA_IRQ_NUM)
#define NVIC_PORTB_IRQ_BIT (1 << NVIC_PORTB_IRQ_NUM)

extern SemaphoreHandle_t GUI_GPIO_IRQ;

void SoC_Init();

#ifdef __cplusplus
}
#endif

#endif /* GUI_SOC_H_ */
