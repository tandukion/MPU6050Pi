#ifndef _I2CPI_H
#define _I2CPI_H

#include <iostream>
#include <cstdint>
#include <bitset>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class I2CPi {
    public:
        static int Setup(uint8_t dev_address);

        /**
         * Read 1 bit from given register address
         * 
         * @param fd            {int}       I2C device file handler
         * @param reg_address   {uint8_t}   register address
         * @param bit_number    {uint8_t}   bit to read (0-7)
         */
        static int8_t ReadBit(int fd, uint8_t reg_address, uint8_t bit_number);
        /**
         * Read bits from given register address 
         * 
         * @param fd            {int}       I2C device file handler
         * @param reg_address   {uint8_t}   register address
         * @param bit_start     {uint8_t}   start bit to read (0-7)
         * @param length        {uint8_t}   length of bits to read
         */
        static int8_t ReadBits(int fd, uint8_t reg_address, uint8_t bit_start, uint8_t length);
        static int8_t ReadByte(int fd, uint8_t reg_address);
        static int16_t ReadWord(int fd, uint8_t reg_address);

        static void WriteBit(int fd, uint8_t reg_address, uint8_t data, uint8_t bit_number);
        static void WriteBits(int fd, uint8_t reg_address, uint8_t data, uint8_t bit_start, uint8_t length);
        static void WriteByte(int fd, uint8_t reg_address, uint8_t data);
        static void WriteWord(int fd, uint8_t reg_address, uint16_t data);
};

#endif