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

bool GPIO_PinSet(Port port, int pin) {

    switch (port) {
    case PORTA:
        memory[ADDR_PORTA_CTRL] |= (1 << pin);
        return true;
        break;
    case PORTB:
        memory[ADDR_PORTB_CTRL] |= (1 << pin);
        return true;
        break;
    case PORTC:
        memory[ADDR_PORTC_CTRL] |= (1 << pin);
        return true;
        break;
    case PORTD:
        memory[ADDR_PORTD_CTRL] |= (1 << pin);
        return true;
        break;
    default:
        return false;
    }
}

bool GPIO_PinClear(Port port, int pin) {
    switch (port) {
    case PORTA:
        memory[ADDR_PORTA_CTRL] &= ~(1 << pin);
        return true;
        break;
    case PORTB:
        memory[ADDR_PORTB_CTRL] &= ~(1 << pin);
        return true;
        break;
    case PORTC:
        memory[ADDR_PORTC_CTRL] &= ~(1 << pin);
        return true;
        break;
    case PORTD:
        memory[ADDR_PORTD_CTRL] &= ~(1 << pin);
        return true;
        break;
    default:
        return false;
    }
}

bool GPIO_PinToggle(Port port, int pin) {
    switch (port) {
    case PORTA:
        memory[ADDR_PORTA_CTRL] ^= (1 << pin);
        return true;
        break;
    case PORTB:
        memory[ADDR_PORTB_CTRL] ^= (1 << pin);
        return true;
        break;
    case PORTC:
        memory[ADDR_PORTC_CTRL] ^= (1 << pin);
        return true;
        break;
    case PORTD:
        memory[ADDR_PORTD_CTRL] ^= (1 << pin);
        return true;
        break;
    default:
        return false;
    }
}

bool GPIO_IntEnable(Port port, int pin) {
    switch (port) {
    case PORTA:
        memory[ADDR_PORTA_INT] |= (1 << pin);
        return true;
        break;
    case PORTB:
        memory[ADDR_PORTB_INT] |= (1 << pin);
        return true;
        break;
    case PORTC:
        memory[ADDR_PORTC_INT] |= (1 << pin);
        return true;
        break;
    case PORTD:
        memory[ADDR_PORTD_INT] |= (1 << pin);
        return true;
        break;
    default:
        return false;
    }
}

bool GPIO_IntDisable(Port port, int pin) {
    switch (port) {
    case PORTA:
        memory[ADDR_PORTA_INT] &= ~(1 << pin);
        return true;
        break;
    case PORTB:
        memory[ADDR_PORTB_INT] &= ~(1 << pin);
        return true;
        break;
    case PORTC:
        memory[ADDR_PORTC_INT] &= ~(1 << pin);
        return true;
        break;
    case PORTD:
        memory[ADDR_PORTD_INT] &= ~(1 << pin);
        return true;
        break;
    default:
        return false;
    }
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

