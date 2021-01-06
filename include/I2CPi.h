#ifndef _I2CPI_H
#define _I2CPI_H

#include <iostream>
#include <cstdint>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class I2CPi {
    public:
        static int Setup(uint8_t dev_address);

        static int8_t ReadByte(int fd, uint8_t reg_address);
        static int16_t ReadWord(int fd, uint8_t reg_address);

        static void WriteByte(int fd, uint8_t reg_address, uint8_t data);
        static void WriteWord(int fd, uint8_t reg_address, uint16_t data);
};

#endif