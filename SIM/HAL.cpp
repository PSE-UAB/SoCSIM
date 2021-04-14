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

    uint32_t address;

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
#if 1
        memory[address] |= (1 << pin);
#else
        uint32_t aux = memory[address];
        aux |= (1 << pin);
        memory[address] = aux;
#endif
    } else {
#if 1
        memory[address] &= ~(1 << pin);
#else
        uint32_t aux = memory[address];
        aux &= ~(1 << pin);
        memory[address] = aux;
#endif
    }

    return true;
}

bool GPIO_PinSet(Port port, uint32_t pin) {

    uint32_t address;
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

#if 1
    memory[address] |= (1 << pin);
#else
    uint32_t aux = memory[address];
    aux |= (1 << pin);
    memory[address] = aux;
#endif

    return true;
}

bool GPIO_PinClear(Port port, uint32_t pin) {
    uint32_t address;
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

#if 1
    memory[address] &= ~(1 << pin);
#else
    uint32_t aux = memory[address];
    aux &= ~(1 << pin);
    memory[address] = aux;
#endif
    return true;
}

bool GPIO_PinToggle(Port port, uint32_t pin) {
    uint32_t address;
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

#if 1
    memory[address] ^= (1 << pin);
#else
    uint32_t aux = memory[address];
    aux ^= (1 << pin);
    memory[address] = aux;
#endif
    return true;
}

bool GPIO_IntEnable(Port port, uint32_t pin) {
    uint32_t address;
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

#if 1
    memory[address] |= (1 << pin);
#else
    uint32_t aux = memory[address];
    aux |= (1 << pin);
    memory[address] = aux;
#endif
    return true;
}

bool GPIO_IntDisable(Port port, uint32_t pin) {
    uint32_t address;
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

#if 1
    memory[address] &= ~(1 << pin);
#else
    uint32_t aux = memory[address];
    aux &= ~(1 << pin);
    memory[address] = aux;
#endif
    return true;
}

bool NVIC_IntClear(uint32_t irq) {

#if 1
    memory[ADDR_NVIC_IRQ] &= ~(1 << irq);
#else
    uint32_t aux = memory[ADDR_NVIC_IRQ];
    aux &= ~(1 << irq);
    memory[ADDR_NVIC_IRQ] = aux;
#endif
    return true;
}

bool HAL_Trace(char c) {
    memory[ADDR_TRACE] = c;
    return true;
}

bool TIMER_PrescalerSet(timer_prescaler_t pres) {
#if 1
    memory[ADDR_TIMER_CTRL] |= (pres & 0xFF) << TIMER_CTRL_PRESCALER_SHIFT;
#else
    uint32_t aux = memory[ADDR_TIMER_CTRL];
    aux |= (pres & 0xFF) << TIMER_CTRL_PRESCALER_SHIFT;
    memory[ADDR_TIMER_CTRL] = aux;
#endif
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
#if 1
    memory[ADDR_TIMER_CTRL] |= 0x01;
#else
    uint32_t aux = memory[ADDR_TIMER_CTRL];
    aux |= 0x01;
    memory[ADDR_TIMER_CTRL] = aux;
#endif
    return true;
}

bool TIMER_Disable() {
#if 1
    memory[ADDR_TIMER_CTRL] &= 0x00;
#else
    uint32_t aux = memory[ADDR_TIMER_CTRL];
    aux &= ~0x01;
    memory[ADDR_TIMER_CTRL] = aux;
#endif
    return true;
}

/************************************ RTC  ***********************************/
bool RTC_Enable() {
#if 1
    memory[ADDR_RTC_CTRL] |= 0x01;
#else
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux |= 0x01;
    memory[ADDR_RTC_CTRL] = aux;
#endif
    return true;
}

bool RTC_Disable() {
#if 1
    memory[ADDR_RTC_CTRL] &= ~0x01;
#else
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux &= ~0x01;
    memory[ADDR_RTC_CTRL] = aux;
#endif
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
#if 1
    memory[ADDR_RTC_CTRL] |= 0x80;
#else
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux |= 0x80;
    memory[ADDR_RTC_CTRL] = aux;
#endif
    return true;
}

bool RTC_IntDisable() {
#if 1
    memory[ADDR_RTC_CTRL] &= ~0x80;
#else
    uint32_t aux = memory[ADDR_RTC_CTRL];
    aux &= ~0x80;
    memory[ADDR_RTC_CTRL] = aux;
#endif
    return true;
}



/************************************ DAC  ***********************************/
bool DAC_Enable() {
    memory[ADDR_DAC_CTRL] |= 0x01;
    return true;
}

bool DAC_Disable() {
    memory[ADDR_DAC_CTRL] &= ~0x01;
    return true;
}

bool DAC_IntEnable() {
    memory[ADDR_DAC_CTRL] |= 0x80;
    return true;
}

bool DAC_IntDisable() {
    memory[ADDR_DAC_CTRL] &= ~0x80;
    return true;
}

bool DAC_Set(uint16_t value) {
    memory[ADDR_DAC_DATA] = value;
    return true;
}

/************************************ UART ***********************************/

bool UART_Enable() {
    memory[ADDR_UART_CTRL] |= 0x01;
    return true;
}


bool UART_Disable() {
    memory[ADDR_UART_CTRL] &= ~0x01;
    return true;
}

bool UART_IntEnable() {
    memory[ADDR_UART_CTRL] |= 0x80;
    return true;
}

bool UART_IntDisable() {
    memory[ADDR_UART_CTRL] &= ~0x80;
    return true;
}

uint32_t UART_GetStatus() {
    return memory[ADDR_UART_STATUS];
}

uint8_t UART_Rx() {
    return (memory[ADDR_UART_RXDATA] & 0x000000FF);
}


bool UART_Tx(uint8_t data) {
    memory[ADDR_UART_TXDATA] = data;
    return true;
}


/************************************ ADC ************************************/
bool ADC_Init(ADC_channel_t ch) {
    uint32_t aux;
    switch (ch) {
        case ADC_CHAN_0:
            aux = (1 << 2);
            break;
        case ADC_CHAN_1:
            aux = (1 << 3);
            break;
        default:
            aux = 0;
            break;
    }

    memory[ADDR_ADC_ADMUX] = aux;
    return true;
}

bool ADC_Enable() {
    memory[ADDR_ADC_CTRL] |= 0x01;
    return true;
}

bool ADC_Disable() {
    memory[ADDR_ADC_CTRL] &= ~0x01;
    return true;
}

bool ADC_IntEnable() {
    memory[ADDR_ADC_CTRL]  |= 0x80;
    return true;
}

bool ADC_IntDisable() {
    memory[ADDR_ADC_CTRL] &= ~0x80;
    return true;
}

uint32_t ADC_GetStatus() {
    return memory[ADDR_ADC_STATUS];
}

uint32_t ADC_GetData() {
    return memory[ADDR_ADC_DATA];
}

#define RTC_CNT  (&memory[ADDR_RTC_CNT])

uint32_t get_test() {
   return *RTC_CNT;
}

/******************************** Memory access ******************************/
void HAL_MemoryWrite(uint32_t addr, uint32_t data) {
    memory[addr] = data;
}

uint32_t HAL_MemoryRead(uint32_t addr) {
    return memory[addr];
}
