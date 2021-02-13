/*!
 \file Memory.h
 \brief Memory
 \author Màrius Montón
 \date Feb 2021
 */
// SPDX-License-Identifier: GPL-3.0-or-later


#ifndef PRAC1_MEMORY_H
#define PRAC1_MEMORY_H

#include <cstdint>
#include <unordered_map>

enum {
    ADDR_PORTA_CTRL = 0x1000,
    ADDR_PORTA_INT  = 0x1004,
    ADDR_PORTA_OUT  = 0x1008,
    ADDR_PORTA_IN   = 0x100C,
    ADDR_PORTB_CTRL = 0x2000,
    ADDR_PORTB_INT  = 0x2004,
    ADDR_PORTB_OUT  = 0x2008,
    ADDR_PORTB_IN   = 0x200C,
    ADDR_PORTC_CTRL = 0x3000,
    ADDR_PORTC_INT  = 0x3004,
    ADDR_PORTC_OUT  = 0x3008,
    ADDR_PORTC_IN   = 0x300C,
    ADDR_PORTD_CTRL = 0x4000,
    ADDR_PORTD_INT  = 0x4004,
    ADDR_PORTD_OUT  = 0x4008,
    ADDR_PORTD_IN   = 0x400C,
    ADDR_NVIC_CTRL  = 0xA000,
    ADDR_NVIC_IRQ   = 0xA004,
    ADDR_I2C0_CTRL  = 0xB000,
    ADDR_TIMER_CTRL = 0xC000,
    ADDR_TIMER_TOP  = 0xC004,
    ADDR_TIMER_CNT  = 0xC008,
    ADDR_TIMER_CMP  = 0xC00C,
    ADDR_TRACE      = 0xF000,
};

extern std::unordered_map<uint32_t, uint32_t> memory;


#endif //PRAC1_MEMORY_H
