/*
 * UART.h
 *
 *  Created on: 1 de març 2021
 *      Author: marius
 */

#ifndef SIM_UART_H_
#define SIM_UART_H_

#include <string>
#include <queue>

class UART {
public:
    explicit UART(int baudrate);
    std::string getDevicename() const;
    int getBaudrate() const;
    void send(uint8_t data) const;
private:
    int fd;
    std::string device_name;
    int baudrate;
    std::queue<uint8_t> tx_buffer;
    std::queue<uint8_t> rx_buffer;

    [[noreturn]] static void *reader(void*);
    static void updateRegister(uint8_t val);
};

#endif /* SIM_UART_H_ */
