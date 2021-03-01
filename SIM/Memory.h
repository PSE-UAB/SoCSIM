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
#include <functional>
#include <iostream>

enum {
    ADDR_PORTA_CTRL = 0x1000,
    ADDR_PORTA_INT = 0x1004,
    ADDR_PORTA_OUT = 0x1008,
    ADDR_PORTA_IN = 0x100C,
    ADDR_PORTB_CTRL = 0x2000,
    ADDR_PORTB_INT = 0x2004,
    ADDR_PORTB_OUT = 0x2008,
    ADDR_PORTB_IN = 0x200C,
    ADDR_PORTC_CTRL = 0x3000,
    ADDR_PORTC_INT = 0x3004,
    ADDR_PORTC_OUT = 0x3008,
    ADDR_PORTC_IN = 0x300C,
    ADDR_PORTD_CTRL = 0x4000,
    ADDR_PORTD_INT = 0x4004,
    ADDR_PORTD_OUT = 0x4008,
    ADDR_PORTD_IN = 0x400C,
    ADDR_NVIC_CTRL = 0xA000,
    ADDR_NVIC_IRQ = 0xA004,
    ADDR_I2C0_CTRL = 0xB000,
    ADDR_TIMER_CTRL = 0xC000,
    ADDR_TIMER_TOP = 0xC004,
    ADDR_TIMER_CNT = 0xC008,
    ADDR_TIMER_CMP = 0xC00C,
    ADDR_RTC_CTRL = 0xD000,
    ADDR_RTC_CNT = 0xD004,
    ADDR_RTC_CMP = 0xD008,
    ADDR_TRACE = 0xF000,
    ADDR_DAC_CTRL = 0x10000,
    ADDR_DAC_DATA = 0x10004,
};

using cb_func = std::function<void(int, int)>;

struct WordMem {
    WordMem() :
            data(0), param_rd(0), param_wr(0), cb_rd(nullptr), cb_wr(nullptr) {
    }

    WordMem& operator=(uint32_t a) {
        if (data != a) {
            data = a;

            if (cb_wr) {
                cb_wr(a, param_wr);
            }
        }
        return *this;
    }

    operator int() const {
        if (cb_rd) {
            cb_rd(data, param_rd);
        }
        return data;
    }

#if 0
    int operator &=(int p_data) {
        if (cb_rd) {
            cb_rd(data, param_wr);
        }

        data = data & p_data;

        if (cb_wr) {
            cb_wr(data, param_wr);
        }

        return data;
    }

    int operator |=(int p_data) {
        data = data | p_data;
        return data;
    }

    int operator ^=(int p_data) {
        data = data ^ p_data;
        return data;
    }
#endif

    bool register_rd_cb(cb_func cb, uint32_t param) {
        cb_rd = cb;
        param_rd = param;
        return true;
    }

    bool register_wr_cb(cb_func cb, uint32_t param) {
        cb_wr = cb;
        param_wr = param;
        return true;
    }

private:
    uint32_t data;
    uint32_t param_rd;
    uint32_t param_wr;
    cb_func cb_rd;
    cb_func cb_wr;
};

extern std::unordered_map<uint32_t, WordMem> memory;

#endif //PRAC1_MEMORY_H
