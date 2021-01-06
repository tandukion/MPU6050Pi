#include "MPU6050Pi.h"

MPU6050Pi::MPU6050Pi(uint8_t address, uint8_t gyro_range, uint8_t accel_range) {
    // Set the I2C address of the device
    I2C_ADDRESS = address;

    // Initialize the I2C device file handler
    fd = wiringPiI2CSetup(I2C_ADDRESS);

    // Disable SLEEP mode
    wiringPiI2CWriteReg8(fd, PWR_MGMT_1, 0x00);

    // Set DLPF (Digital Low Pass Filter) to 44Hz. Check table in Register Map.
    MPU6050Pi::SetDLPFMode(DLPF_BW_44);

    // Set sample rate divider to 200Hz, div=5.
    MPU6050Pi::SetRate(0x05);

    // Configure gyroscope setting
    MPU6050Pi::SetFullScaleGyroRange();

    // Configure accelerometer setting
    MPU6050Pi::SetFullScaleAccelRange();

    // Reset offset to zero
    wiringPiI2CWriteReg8(fd, XA_OFFS_H, 0x00);
    wiringPiI2CWriteReg8(fd, XA_OFFS_L, 0x00);
    wiringPiI2CWriteReg8(fd, YA_OFFS_H, 0x00);
    wiringPiI2CWriteReg8(fd, YA_OFFS_L, 0x00);
    wiringPiI2CWriteReg8(fd, ZA_OFFS_H, 0x00);
    wiringPiI2CWriteReg8(fd, ZA_OFFS_L, 0x00);
    wiringPiI2CWriteReg8(fd, XA_OFFS_H, 0x00);
    wiringPiI2CWriteReg8(fd, XA_OFFS_H, 0x00);
    wiringPiI2CWriteReg8(fd, XA_OFFS_H, 0x00);
}

void MPU6050Pi::SetRate(uint8_t rate) {
    wiringPiI2CWriteReg8(fd, SMPLRT_DIV, rate);
}

void MPU6050Pi::SetDLPFMode(uint8_t mode) {
    wiringPiI2CWriteReg8(fd, CONFIG, mode);
}

void MPU6050Pi::SetFullScaleGyroRange(uint8_t range) {
    uint8_t gyro_config_val;
    switch (range) {
        case 0:         // 250 deg/s full scale range
            gyro_sensitivity = 131.0;
            gyro_config_val = 0x00;
            break;
        case 1:         // 500 deg/s full scale range
            gyro_sensitivity = 65.5;
            gyro_config_val = 0x08;
            break;
        case 2:         // 1000 deg/s full scale range
            gyro_sensitivity = 32.8;
            gyro_config_val = 0x10;
            break;
        case 3:         // 2000 deg/s full scale range
            gyro_sensitivity = 16.4;
            gyro_config_val = 0x18;
            break;
    }
    wiringPiI2CWriteReg8(fd, GYRO_CONFIG, gyro_config_val);
}

void MPU6050Pi::SetFullScaleAccelRange(uint8_t range) {
    uint8_t accel_config_val;
    switch (range) {
        case 0x00:      // 250 deg/s full scale range
            accel_sensitivity = 16384.0;
            accel_config_val = 0x00;
            break;
        case 0x01:      // 500 deg/s full scale range
            accel_sensitivity = 8192.0;
            accel_config_val = 0x08;
            break;
        case 0x02:      // 1000 deg/s full scale range
            accel_sensitivity = 4096.0;
            accel_config_val = 0x10;
            break;
        case 0x03:      // 2000 deg/s full scale range
            accel_sensitivity = 2048.0;
            accel_config_val = 0x18;
            break;
    }
    wiringPiI2CWriteReg8(fd, ACCEL_CONFIG, accel_config_val);
}