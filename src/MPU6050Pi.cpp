#include "MPU6050Pi.h"

MPU6050Pi::MPU6050Pi() {
    // Set default the I2C address of the device
    I2C_address_ = 0x68;

    // Initialize the I2C device file handler
    fd_ = I2CPi::Setup(0x68);

    // Disable SLEEP mode
    I2CPi::WriteByte(fd_, PWR_MGMT_1, 0x00);

    // Set DLPF (Digital Low Pass Filter) to 44Hz. Check table in Register Map.
    MPU6050Pi::SetDLPFMode(DLPF_BW_44);

    // Set sample rate divider to 200Hz, div=5.
    MPU6050Pi::SetRate(0x05);

    // Configure gyroscope setting with default range
    MPU6050Pi::SetFullScaleGyroRange(0);

    // Configure accelerometer setting with default range
    MPU6050Pi::SetFullScaleAccelRange(0);

    // Set offsets with default zeros
    MPU6050Pi::SetXAccelOffset(0x0000);
    MPU6050Pi::SetYAccelOffset(0x0000);
    MPU6050Pi::SetZAccelOffset(0x0000);
    MPU6050Pi::SetXGyroOffset(0x0000);
    MPU6050Pi::SetYGyroOffset(0x0000);
    MPU6050Pi::SetZGyroOffset(0x0000);
}

MPU6050Pi::MPU6050Pi(int16_t *offsets) {
    // Set default the I2C address of the device
    I2C_address_ = 0x68;

    // Initialize the I2C device file handler
    fd_ = I2CPi::Setup(0x68);

    // Disable SLEEP mode
    I2CPi::WriteByte(fd_, PWR_MGMT_1, 0x00);

    // Set DLPF (Digital Low Pass Filter) to 44Hz. Check table in Register Map.
    MPU6050Pi::SetDLPFMode(DLPF_BW_44);

    // Set sample rate divider to 200Hz, div=5.
    MPU6050Pi::SetRate(0x05);

    // Configure gyroscope setting with default range
    MPU6050Pi::SetFullScaleGyroRange(0);

    // Configure accelerometer setting with default range
    MPU6050Pi::SetFullScaleAccelRange(0);

    // Set offsets
    MPU6050Pi::SetXAccelOffset(offsets[0]);
    MPU6050Pi::SetYAccelOffset(offsets[1]);
    MPU6050Pi::SetZAccelOffset(offsets[2]);
    MPU6050Pi::SetXGyroOffset(offsets[4]);
    MPU6050Pi::SetYGyroOffset(offsets[5]);
    MPU6050Pi::SetZGyroOffset(offsets[6]);
}

void MPU6050Pi::SetRate(uint8_t rate) {
    I2CPi::WriteByte(fd_, SMPLRT_DIV, rate);
}

void MPU6050Pi::SetDLPFMode(uint8_t mode) {
    I2CPi::WriteByte(fd_, CONFIG, mode);
}

void MPU6050Pi::SetFullScaleGyroRange(uint8_t range) {
    uint8_t gyro_config_val;
    switch (range) {
        case 0:         // 250 deg/s full scale range
            gyro_sensitivity_ = 131.0;
            gyro_config_val = 0x00;
            break;
        case 1:         // 500 deg/s full scale range
            gyro_sensitivity_ = 65.5;
            gyro_config_val = 0x08;
            break;
        case 2:         // 1000 deg/s full scale range
            gyro_sensitivity_ = 32.8;
            gyro_config_val = 0x10;
            break;
        case 3:         // 2000 deg/s full scale range
            gyro_sensitivity_ = 16.4;
            gyro_config_val = 0x18;
            break;
    }
    I2CPi::WriteByte(fd_, GYRO_CONFIG, gyro_config_val);
}

void MPU6050Pi::SetFullScaleAccelRange(uint8_t range) {
    uint8_t accel_config_val;
    switch (range) {
        case 0x00:      // 250 deg/s full scale range
            accel_sensitivity_ = 16384.0;
            accel_config_val = 0x00;
            break;
        case 0x01:      // 500 deg/s full scale range
            accel_sensitivity_ = 8192.0;
            accel_config_val = 0x08;
            break;
        case 0x02:      // 1000 deg/s full scale range
            accel_sensitivity_ = 4096.0;
            accel_config_val = 0x10;
            break;
        case 0x03:      // 2000 deg/s full scale range
            accel_sensitivity_ = 2048.0;
            accel_config_val = 0x18;
            break;
    }
    I2CPi::WriteByte(fd_, ACCEL_CONFIG, accel_config_val);
}

void MPU6050Pi::SetOffset(int16_t *offset){
    I2CPi::WriteWord(fd_, XA_OFFS_H, offset[0]);
    I2CPi::WriteWord(fd_, YA_OFFS_H, offset[1]);
    I2CPi::WriteWord(fd_, ZA_OFFS_H, offset[2]);
    I2CPi::WriteWord(fd_, XG_OFFSET_H, offset[3]);
    I2CPi::WriteWord(fd_, YG_OFFSET_H, offset[4]);
    I2CPi::WriteWord(fd_, ZG_OFFSET_H, offset[5]);
}
void MPU6050Pi::SetAccelOffset(int16_t *offset){
    I2CPi::WriteWord(fd_, XA_OFFS_H, offset[0]);
    I2CPi::WriteWord(fd_, YA_OFFS_H, offset[1]);
    I2CPi::WriteWord(fd_, ZA_OFFS_H, offset[2]);
}

void MPU6050Pi::SetXAccelOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, XA_OFFS_H, offset);
}

void MPU6050Pi::SetYAccelOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, YA_OFFS_H, offset);
}

void MPU6050Pi::SetZAccelOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, ZA_OFFS_H, offset);
}

void MPU6050Pi::SetGyroOffset(int16_t *offset){
    I2CPi::WriteWord(fd_, XG_OFFSET_H, offset[0]);
    I2CPi::WriteWord(fd_, YG_OFFSET_H, offset[1]);
    I2CPi::WriteWord(fd_, ZG_OFFSET_H, offset[2]);
}
void MPU6050Pi::SetXGyroOffset(int16_t offset){
    I2CPi::WriteWord(fd_, XG_OFFSET_H, offset);
}

void MPU6050Pi::SetYGyroOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, YG_OFFSET_H, offset);
}

void MPU6050Pi::SetZGyroOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, ZG_OFFSET_H, offset);
}

void MPU6050Pi::GetMotion6(int16_t* ax, int16_t* ay, int16_t* az,
                           int16_t* gx, int16_t* gy, int16_t* gz) {
    *ax = I2CPi::ReadWord(fd_, ACCEL_XOUT_H);
    *ay = I2CPi::ReadWord(fd_, ACCEL_YOUT_H);
    *az = I2CPi::ReadWord(fd_, ACCEL_ZOUT_H);
    *gx = I2CPi::ReadWord(fd_, GYRO_XOUT_H);
    *gy = I2CPi::ReadWord(fd_, GYRO_YOUT_H);
    *gz = I2CPi::ReadWord(fd_, GYRO_ZOUT_H);
}

void MPU6050Pi::GetAccel(int16_t* ax, int16_t* ay, int16_t* az) {
    *ax = I2CPi::ReadWord(fd_, ACCEL_XOUT_H);
    *ay = I2CPi::ReadWord(fd_, ACCEL_YOUT_H);
    *az = I2CPi::ReadWord(fd_, ACCEL_ZOUT_H);
}

void MPU6050Pi::GetAccelY(int16_t* ay) {
    *ay = I2CPi::ReadWord(fd_, ACCEL_YOUT_H);
}

void MPU6050Pi::GetAccelFloat(float* ax, float* ay, float* az) {
    int16_t x, y, z;
    MPU6050Pi::GetAccel(&x, &y, &z);
    *ax = (float) x / accel_sensitivity_;
    *ay = (float) y / accel_sensitivity_;
    *az = (float) z / accel_sensitivity_;
}

void MPU6050Pi::GetGyro(int16_t* gx, int16_t* gy, int16_t* gz) {
    *gx = I2CPi::ReadWord(fd_, GYRO_XOUT_H);
    *gy = I2CPi::ReadWord(fd_, GYRO_YOUT_H);
    *gz = I2CPi::ReadWord(fd_, GYRO_ZOUT_H);
}

void MPU6050Pi::GetGyroFloat(float* gx, float* gy, float* gz) {
    int16_t x, y, z;
    MPU6050Pi::GetGyro(&x, &y, &z);
    *gx = (float) x / gyro_sensitivity_;
    *gy = (float) y / gyro_sensitivity_;
    *gz = (float) z / gyro_sensitivity_;
}


float MPU6050Pi::GetAccelSensitivity() {
    return accel_sensitivity_;
}
float MPU6050Pi::GetGyroSensitivity() {
    return gyro_sensitivity_;
}