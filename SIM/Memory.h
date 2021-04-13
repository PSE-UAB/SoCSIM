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
#include <utility>

enum {
    ADDR_PORTA_CTRL  = 0x01000,
    ADDR_PORTA_INT   = 0x01004,
    ADDR_PORTA_OUT   = 0x01008,
    ADDR_PORTA_IN    = 0x0100C,
    ADDR_PORTB_CTRL  = 0x02000,
    ADDR_PORTB_INT   = 0x02004,
    ADDR_PORTB_OUT   = 0x02008,
    ADDR_PORTB_IN    = 0x0200C,
    ADDR_PORTC_CTRL  = 0x03000,
    ADDR_PORTC_INT   = 0x03004,
    ADDR_PORTC_OUT   = 0x03008,
    ADDR_PORTC_IN    = 0x0300C,
    ADDR_PORTD_CTRL  = 0x04000,
    ADDR_PORTD_INT   = 0x04004,
    ADDR_PORTD_OUT   = 0x04008,
    ADDR_PORTD_IN    = 0x0400C,
    ADDR_NVIC_CTRL   = 0x0A000,
    ADDR_NVIC_IRQ    = 0x0A004,
    ADDR_I2C0_CTRL   = 0x0B000,
    ADDR_TIMER_CTRL  = 0x0C000,
    ADDR_TIMER_TOP   = 0x0C004,
    ADDR_TIMER_CNT   = 0x0C008,
    ADDR_TIMER_CMP   = 0x0C00C,
    ADDR_RTC_CTRL    = 0x0D000,
    ADDR_RTC_CNT     = 0x0D004,
    ADDR_RTC_CMP     = 0x0D008,
    ADDR_TRACE       = 0x0F000,
    ADDR_DAC_CTRL    = 0x10000,
    ADDR_DAC_DATA    = 0x10004,
    ADDR_UART_CTRL   = 0x20000,
    ADDR_UART_STATUS = 0x20004,
    ADDR_UART_TXDATA = 0x20008,
    ADDR_UART_RXDATA = 0x2000C,
    ADDR_ADC_ADMUX   = 0x30000,
    ADDR_ADC_CTRL    = 0x30004,
    ADDR_ADC_DATA    = 0x30008,
    ADDR_ADC_STATUS  = 0x3000C,
};

/**
 * @brief definition of callback function type
 */
using cb_func = std::function<void(uint32_t, uint32_t)>;

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

    operator uint32_t() const {
        if (cb_rd) {
            cb_rd(data, param_rd);
        }
        return data;
    }

    uint32_t operator &=(int p_data) {
        if (cb_rd) {
            cb_rd(data, param_wr);
        }

        data = data & p_data;

        if (cb_wr) {
            cb_wr(data, param_wr);
        }

        return data;
    }

    uint32_t operator |=(int p_data) {
        if (cb_rd) {
            cb_rd(data, param_wr);
        }

        data = data | p_data;

        if (cb_wr) {
            cb_wr(data, param_wr);
        }

        return data;
    }

    uint32_t operator ^=(int p_data) {

        if (cb_rd) {
            cb_rd(data, param_wr);
        }

        data = data ^ p_data;

        if (cb_wr) {
            cb_wr(data, param_wr);
        }
        return data;
    }

    /**
     * @brief Registers callback function for a memory address
     * @param cb function to call when memory read
     * @param param parameter to send to the callback function
     * @return true
     */
    bool register_rd_cb(cb_func cb, uint32_t param) {
        cb_rd = std::move(cb);
        param_rd = param;
        return true;
    }

    /**
     * @brief Registers callback function for a memory address
     * @param cb function to call when memory written
     * @param param parameter to send to the callback function
     * @return true
     */
    bool register_wr_cb(cb_func cb, uint32_t param) {
        cb_wr = std::move(cb);
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
