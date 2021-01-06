#include "I2CPi.h"

int I2CPi::Setup(uint8_t dev_address) {
    // Initialize the I2C device file handler
    return wiringPiI2CSetup(dev_address);
}

int8_t I2CPi::ReadByte(int fd, uint8_t reg_address){
    return (int8_t) wiringPiI2CReadReg8(fd, reg_address);
}
int16_t I2CPi::ReadWord(int fd, uint8_t reg_address){
    int high = wiringPiI2CReadReg8(fd, reg_address);
    int low = wiringPiI2CReadReg8(fd, reg_address+1);
    int16_t val = (high << 8) + low;
    // std::cout << std::hex << high << " " << low << " " << val << std::endl;
    return val;
}

void I2CPi::WriteByte(int fd, uint8_t reg_address, uint8_t data) {
    wiringPiI2CWriteReg8(fd, reg_address, data);
}
void I2CPi::WriteWord(int fd, uint8_t reg_address, uint16_t data) {
    uint8_t ms_byte = (uint8_t) (data >> 8);
    uint8_t ls_byte = (uint8_t) (data >> 0);

    I2CPi::WriteByte(fd, reg_address, ms_byte);
    I2CPi::WriteByte(fd, reg_address+1, ls_byte);
}
