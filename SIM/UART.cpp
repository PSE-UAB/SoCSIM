/*
 * UART.cpp
 *
 *  Created on: 1 de març 2021
 *      Author: marius
 */

#include "UART.h"
#include "Memory.h"
#include "SoC.h"
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <thread>

typedef void * (*THREADFUNCPTR)(void *);

UART::UART(int m_baudrate): baudrate(m_baudrate) {
  
  // taken from https://github.com/cymait/virtual-serial-port-example
  // al credits to him/her
  
  fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
  grantpt(fd);
  unlockpt(fd);
  device_name = ptsname(fd);
  
  struct termios newtio = {};
  memset(&newtio, 0, sizeof(newtio));
  struct termios oldtio ={};
  tcgetattr(fd, &oldtio);

  newtio = oldtio;
  newtio.c_cflag = baudrate | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = 0;
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;
  newtio.c_cc[VMIN] = 1;
  newtio.c_cc[VTIME] = 0;
  tcflush(fd, TCIFLUSH);

  cfsetispeed(&newtio, baudrate);
  cfsetospeed(&newtio, baudrate);
  tcsetattr(fd, TCSANOW, &newtio);

  std::cout << "CTOR FD: " << fd << std::endl;
  pthread_t thread;
  pthread_create(&thread, nullptr, (THREADFUNCPTR) &UART::reader, &fd);
}

std::string UART::getDevicename() const{
  return device_name;
}

int UART::getBaudrate() const {
  return baudrate;
}

void UART::send(uint8_t data) const {
    [[maybe_unused]] ssize_t i = write(fd, &data, 1);
}


[[noreturn]] void *UART::reader(void* param) {
    int fd = *(int*) param;

    while (true) {
        char inputbyte;
        if (read(fd, &inputbyte, 1) == 1) {
            updateRegister(inputbyte);
            std::cout << "UART: " << inputbyte << std::endl;
        }
    }
}

void UART::updateRegister(uint8_t val) {
    memory[ADDR_UART_TXDATA] = val;
    UART_NotifyRxData();
}