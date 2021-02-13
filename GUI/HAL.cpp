/*!
 \file HAL.cpp
 \brief  HAL
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later

#include "HAL.h"
#include "Memory.h"
#include "SoC.h"

bool GPIO_PinCfg(Port port, int pin, bool out) {

    uint32_t address = 0;

    switch (port) {
    case PORTA:
        address = ADDR_PORTA_CTRL;
        break;
    case PORTB:
        address = ADDR_PORTB_CTRL;
        break;
    case PORTC:
        address = ADDR_PORTC_CTRL;
        break;
    case PORTD:
        address = ADDR_PORTD_CTRL;
        break;
    default:
        return false;
    }

    if (out == true) {
        memory[address] |= (1 << pin);
    } else {
        memory[address] &= ~(1 << pin);
    }

    return true;
}

bool GPIO_PinSet(Port port, int pin) {

    uint32_t address = 0;
    switch (port) {
    case PORTA:
        address = ADDR_PORTA_OUT;
        break;
    case PORTB:
        address = ADDR_PORTB_OUT;
        break;
    case PORTC:
        address = ADDR_PORTC_OUT;
        break;
    case PORTD:
        address = ADDR_PORTD_OUT;
        break;
    default:
        return false;
    }

    memory[address] |= (1 << pin);
    return true;
}

bool GPIO_PinClear(Port port, int pin) {
    uint32_t address = 0;
    switch (port) {
    case PORTA:
        address = ADDR_PORTA_OUT;
        break;
    case PORTB:
        address = ADDR_PORTB_OUT;
        break;
    case PORTC:
        address = ADDR_PORTC_OUT;
        break;
    case PORTD:
        address = ADDR_PORTD_OUT;
        break;
    default:
        return false;
    }

    memory[address] &= ~(1 << pin);
    return true;
}

bool GPIO_PinToggle(Port port, int pin) {
    uint32_t address = 0;
    switch (port) {
    case PORTA:
        address = ADDR_PORTA_OUT;
        break;
    case PORTB:
        address = ADDR_PORTB_OUT;
        break;
    case PORTC:
        address = ADDR_PORTC_OUT;
        break;
    case PORTD:
        address = ADDR_PORTD_OUT;
        break;
    default:
        return false;
    }

    memory[address] ^= (1 << pin);
    return true;
}

bool GPIO_IntEnable(Port port, int pin) {
    uint32_t address = 0;
    switch (port) {
    case PORTA:
        address = ADDR_PORTA_INT;
        break;
    case PORTB:
        address = ADDR_PORTD_INT;
        break;
    case PORTC:
        address = ADDR_PORTC_INT;
        break;
    case PORTD:
        address = ADDR_PORTD_INT;
        break;
    default:
        return false;
    }

    memory[address] |= (1 << pin);
    return true;
}

bool GPIO_IntDisable(Port port, int pin) {
    uint32_t address = 0;
     switch (port) {
     case PORTA:
         address = ADDR_PORTA_INT;
         break;
     case PORTB:
         address = ADDR_PORTD_INT;
         break;
     case PORTC:
         address = ADDR_PORTC_INT;
         break;
     case PORTD:
         address = ADDR_PORTD_INT;
         break;
     default:
         return false;
     }

     memory[address] &= ~(1 << pin);
     return true;
}

bool NVIC_IntClear(int irq) {
    memory[ADDR_NVIC_IRQ] &= ~(1 << irq);
    return true;
}

bool HAL_Trace(char c) {
    memory[ADDR_TRACE] = c;
    return true;
}

bool TIMER_PrescalerSet(int pres) {
    memory[ADDR_TIMER_CTRL] |= (pres & 0xFF) << TIMER_CTRL_PRESCALER_SHIFT;
    return true;
}

int TIMER_PrescalerGet() {
    int pres = memory[ADDR_TIMER_CTRL];
    pres = pres >> (TIMER_CTRL_PRESCALER_SHIFT) & 0xFF;

    return pres;
}

bool TIMER_SetTOP(int top) {
    memory[ADDR_TIMER_TOP] = top;
    return true;
}

bool TIMER_SetCMP(int cmp) {
    memory[ADDR_TIMER_CMP] = cmp;
    return true;
}

bool TIMER_Enable() {
    memory[ADDR_TIMER_CTRL] |= 0x01;
    return true;
}

bool TIMER_Disable() {
    memory[ADDR_TIMER_CTRL] &= 0x00;
    return true;
}

