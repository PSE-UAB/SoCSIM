/*!
 \file Drivers.cpp
 \brief Drivers
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


#include "GUI.h"
#include "drivers.h"

#ifdef __cplusplus
extern "C" {
#endif

    void GreenLED_On() {
       // MemWrite(MEM_PORTB, 1);
    }

    void GreenLED_Off() {
        // MemWrite(MEM_PORTB, 0);
    }

    void GreenLED_Toggle() {
//        uint32_t aux = MemRead(MEM_PORTB);
//        aux = !aux;
//        MemWrite(MEM_PORTB, aux);
    }

    void RedLED_On() {
        //MemWrite(MEM_PORTA, 1);
    }

    void RedLED_Off() {
        //MemWrite(MEM_PORTA, 0);
    }

    void RedLED_Toggle() {
//        uint32_t aux = MemRead(MEM_PORTA);
//        aux = !aux;
//        MemWrite(MEM_PORTA, aux);
    }


#ifdef __cplusplus
}
#endif
