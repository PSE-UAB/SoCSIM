/*!
 \file Drivers.h
 \brief Drivers
 \author Màrius Montón
 \date Feb 2021
 */

#ifndef DRIVERS_H_
#define DRIVERS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

#include "SIM/GUI.h"
#include "SIM/SoC.h"
#include "SIM/HAL.h"

void GreenLED_On();
void GreenLED_Off();
void GreenLED_Toggle();
void RedLED_On();
void RedLED_Off();
void RedLED_Toggle();


#endif
