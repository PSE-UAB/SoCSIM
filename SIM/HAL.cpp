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

bool GPIO_PinCfg(Port port, uint32_t pin, bool out) {

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
        //memory[address] |= (1 << pin);
        uint32_t aux = memory[address];
        aux |= (1 << pin);
        memory[address] = aux;
    } else {
        //memory[address] &= ~(1 << pin);
        uint32_t aux = memory[address];
        aux &= ~(1 << pin);
        memory[address] = aux;
    }

    return true;
}

bool GPIO_PinSet(Port port, uint32_t pin) {

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

    // memory[address] |= (1 << pin);
    uint32_t aux = memory[address];
    aux |= (1 << pin);
    memory[address] = aux;
    return true;
}

bool GPIO_PinClear(Port port, uint32_t pin) {
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

    // memory[address] &= ~(1 << pin);
    uint32_t aux = memory[address];
    aux &= ~(1 << pin);
    memory[address] = aux;

    return true;
}

bool GPIO_PinToggle(Port port, uint32_t pin) {
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

    // memory[address] ^= (1 << pin);
    uint32_t aux = memory[address];
    aux ^= (1 << pin);
    memory[address] = aux;

    return true;
}

bool GPIO_IntEnable(Port port, uint32_t pin) {
    uint32_t address = 0;
    switch (port) {
    case PORTA:
        address = ADDR_PORTA_INT;
        break;
    case PORTB:
        address = ADDR_PORTB_INT;
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

    // memory[address] |= (1 << pin);
    uint32_t aux = memory[address];
    aux |= (1 << pin);
    memory[address] = aux;
    return true;
}

bool GPIO_IntDisable(Port port, uint32_t pin) {
    uint32_t address = 0;
    switch (port) {
    case PORTA:
        address = ADDR_PORTA_INT;
        break;
    case PORTB:
        address = ADDR_PORTB_INT;
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

    // memory[address] &= ~(1 << pin);
    uint32_t aux = memory[address];
    aux &= ~(1 << pin);
    memory[address] = aux;
    return true;
}

bool NVIC_IntClear(uint32_t irq) {
    // memory[ADDR_NVIC_IRQ] &= ~(1 << irq)
    uint32_t aux = memory[ADDR_NVIC_IRQ];
    aux &= ~(1 << irq);
    memory[ADDR_NVIC_IRQ] = aux;
    return true;
}

bool HAL_Trace(char c) {
    memory[ADDR_TRACE] = c;
    return true;
}

bool TIMER_PrescalerSet(timer_prescaler_t pres) {
    // memory[ADDR_TIMER_CTRL] |= (pres & 0xFF) << TIMER_CTRL_PRESCALER_SHIFT;
    uint32_t aux = memory[ADDR_TIMER_CTRL];
    aux |= (pres & 0xFF) << TIMER_CTRL_PRESCALER_SHIFT;
    memory[ADDR_TIMER_CTRL] = aux;

    return true;
}

timer_prescaler_t TIMER_PrescalerGet() {
    uint32_t pres = memory[ADDR_TIMER_CTRL];
    pres = pres >> (TIMER_CTRL_PRESCALER_SHIFT) & 0xFF;

    return (timer_prescaler_t) pres;
}

bool TIMER_SetTOP(uint32_t top) {
    memory[ADDR_TIMER_TOP] = top;
    return true;
}

bool TIMER_SetCMP(uint32_t cmp) {
    memory[ADDR_TIMER_CMP] = cmp;
    return true;
}

bool TIMER_Enable() {
    // memory[ADDR_TIMER_CTRL] |= 0x01;
    uint32_t aux = memory[ADDR_TIMER_CTRL];
    aux |= 0x01;
    memory[ADDR_TIMER_CTRL] = aux;

    return true;
}

bool TIMER_Disable() {
    // memory[ADDR_TIMER_CTRL] &= 0x00;
    uint32_t aux = memory[ADDR_TIMER_CTRL];
    aux &= ~0x01;
    memory[ADDR_TIMER_CTRL] = aux;
    return true;
}

/************************************ RTC  ***********************************/
bool RTC_Enable() {
    //memory[ADDR_RTC_CTRL] |= 0x01;
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux |= 0x01;
    memory[ADDR_RTC_CTRL] = aux;

    return true;
}

bool RTC_Disable() {
    //memory[ADDR_RTC_CTRL] &= ~0x01;

    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux &= ~0x01;
    memory[ADDR_RTC_CTRL] = aux;

    return true;
}

bool RTC_CompareSet(uint32_t value) {
    memory[ADDR_RTC_CMP] = value;
    return true;
}

uint32_t RTC_CompareGet() {
    return memory[ADDR_RTC_CMP];
}

bool RTC_CounterSet(uint32_t value) {
    memory[ADDR_RTC_CNT] = value;
    return true;
}

uint32_t RTC_CounterGet() {
    return memory[ADDR_RTC_CNT];
}

bool RTC_IntEnable() {
    // memory[ADDR_RTC_CTRL] |= 0x80;
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux |= 0x80;
    memory[ADDR_RTC_CTRL] = aux;

    return true;
}

bool RTC_IntDisable() {
    //memory[ADDR_RTC_CTRL] &= ~0x80;
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux &= ~0x80;
    memory[ADDR_RTC_CTRL] = aux;

    return true;
}


void HAL_MemoryWrite(uint32_t addr, uint32_t data) {
    memory[addr] = data;
}

uint32_t HAL_MemoryRead(uint32_t addr) {
    return memory[addr];
}
