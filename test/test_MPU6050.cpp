#include <iostream>	
#include <iomanip>

#include <wiringPi.h>
#include <wiringPiI2C.h>

const int I2C_ADDR = 0x68;
const int PWR_MGMT_1 = 0x6B;

float read_word_2c(int fd, int addr) {
    int high = wiringPiI2CReadReg8(fd, addr);
    int low = wiringPiI2CReadReg8(fd, addr+1);
    int val = (high << 8) + low;
    return float((val >= 0x8000) ? -((65535 - val) + 1) : val);
}
int main(int argc, char **argv) {

    // Connect to device.
    int fd = wiringPiI2CSetup(I2C_ADDR);
    if (fd == -1) {
        std::cout << "no i2c device found?\n";
        return -1;
    }
    // Device starts in sleep mode so wake it up.
    wiringPiI2CWriteReg16(fd, PWR_MGMT_1, 0);

    // Start ROS node stuff
    // Publish in loop.
    double ax, ay, az, gx, gy, gz;
    while(1) {
        
        // At default sensitivity of 250deg/s we need to scale by 131.
        gx = read_word_2c(fd, 0x43) / 131;
        gy = read_word_2c(fd, 0x45) / 131;
        gz = read_word_2c(fd, 0x47) / 131;
        // Read accelerometer values.
        // At default sensitivity of 2g we need to scale by 16384.
        // Note: at "level" x = y = 0 but z = 1 (i.e. gravity)
        // But! Imu msg docs say acceleration should be in m/2 so need to *9.807
        const float la_rescale = 16384.0 / 9.807;
        ax = read_word_2c(fd, 0x3b) / la_rescale;
        ay = read_word_2c(fd, 0x3d) / la_rescale;
        az = read_word_2c(fd, 0x3f) / la_rescale;

        std::cout << std::fixed << std::setprecision(6) << std::setfill(' ');
        // std::cout << std::setw(9) << gx << " ";
        // std::cout << std::setw(9) << gy << " ";
        // std::cout << std::setw(9) << gz << "\n";
        std::cout << std::setw(9) << ax << " ";
        std::cout << std::setw(9) << ay << " ";
        std::cout << std::setw(9) << az << "\n";
    }
    return 0;
}