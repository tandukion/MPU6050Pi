#include "I2CPi.h"

int I2CPi::Setup(uint8_t dev_address) {
    // Initialize the I2C device file handler
    return wiringPiI2CSetup(dev_address);
}

int8_t I2CPi::ReadBit(int fd, uint8_t reg_address, uint8_t bit_number){
    int8_t b = I2CPi::ReadByte(fd, reg_address);
    int8_t mask = 1<<bit_number;

    // std::cout << "Byte: " << (int)b << std::endl;
    // std::cout << "Bit" << (int)bit_number << ": " << (int)((b & mask) >> bit_number) << std::endl;
    return (b & mask) >> bit_number;
}

int8_t I2CPi::ReadBits(int fd, uint8_t reg_address, uint8_t bit_start, uint8_t length){
    int8_t b = I2CPi::ReadByte(fd, reg_address);
    // Create masking from bit_start and length
    // e.g: (3,4) -> mask = 0b01111000
    int8_t mask = (int8_t)pow(2,length) - 1 << bit_start;

    // std::cout << "Byte: " << (int)b << std::endl;
    // std::cout << "Bit(" << (int)bit_start << "," << (int)length << "): " << (int)((b & mask) >> bit_start) << std::endl;
    return (b & mask) >> bit_start;
}

int8_t I2CPi::ReadByte(int fd, uint8_t reg_address){
    return (int8_t) wiringPiI2CReadReg8(fd, reg_address);
}

int8_t* I2CPi::ReadBytes(int fd, uint8_t reg_address, uint8_t length){
    int8_t *data = new int8_t[length];
    int i;
    for (i=0;i<length;i++){
        data[i] = wiringPiI2CReadReg8(fd, reg_address+i);
    }
    return data;
}

int16_t I2CPi::ReadWord(int fd, uint8_t reg_address){
    int high = wiringPiI2CReadReg8(fd, reg_address);
    int low = wiringPiI2CReadReg8(fd, reg_address+1);
    int16_t val = ((int16_t)high << 8) | low;

    // std::cout << std::hex << high << " " << low << " " << val << std::endl;
    return val;
}

void I2CPi::WriteBit(int fd, uint8_t reg_address, uint8_t bit_number, uint8_t data){
    uint8_t b = I2CPi::ReadByte(fd, reg_address);
    uint8_t write_data = (data == 1) ? (b | (1<<bit_number)) : (b & ~(1 << bit_number));

    // std::cout << "Byte: " << std::bitset<8>(b) << std::endl;
    // std::cout << "write_data (" << (int)data << "," << (int)bit_number << "): " << std::bitset<8>(write_data) << std::endl;
    I2CPi::WriteByte(fd, reg_address, write_data);
}

void I2CPi::WriteBits(int fd, uint8_t reg_address, uint8_t bit_start, uint8_t length, uint8_t data){
    uint8_t b = I2CPi::ReadByte(fd, reg_address);
    // std::cout << "Byte: " << std::bitset<8>(b) << std::endl;

    // Create masking based data, bit_start, and length
    uint8_t mask = (uint8_t)pow(2,length) - 1 << bit_start;
    // std::cout << "Mask: " << std::bitset<8>(mask) << std::endl;

    // Mask current data
    uint8_t write_data = b & ~mask;
    // std::cout << "Masked data: " << std::bitset<8>(write_data) << std::endl;

    // Add data to masked data
    write_data |= data << bit_start;
    // std::cout << "write_data: " << std::bitset<8>(write_data) << std::endl;
    I2CPi::WriteByte(fd, reg_address, write_data);
}

void I2CPi::WriteByte(int fd, uint8_t reg_address, uint8_t data) {
    wiringPiI2CWriteReg8(fd, reg_address, data);
}

void I2CPi::WriteBytes(int fd, uint8_t reg_address, uint8_t *data, uint8_t length){
    int i;
    for (i=0;i<length;i++){
        I2CPi::WriteByte(fd, reg_address+i, data[i]);
    }
}

void I2CPi::WriteWord(int fd, uint8_t reg_address, uint16_t data) {
    uint8_t ms_byte = (uint8_t) (data >> 8);
    uint8_t ls_byte = (uint8_t) (data >> 0);

    I2CPi::WriteByte(fd, reg_address, ms_byte);
    I2CPi::WriteByte(fd, reg_address+1, ls_byte);
}
