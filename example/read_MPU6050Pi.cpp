#include <iostream>	
#include <iomanip>

#include <MPU6050Pi.h>

int main(int argc, char **argv) {

    // Connect to device.
    MPU6050Pi mpu;

    // Start ROS node stuff
    // Publish in loop.
    float ax, ay, az, gx, gy, gz;
    while(1) {
        
        // Get gyroscope data.
        // The result is already scaled by the sensitivity.
        mpu.GetGyro(&gx, &gy, &gz);

        // Get accelerometer values.
        // The result is scaled to its default range +- 2g.
        mpu.GetAccel(&ax, &ay, &az);

        std::cout << std::fixed << std::setprecision(6) << std::setfill(' ');
        std::cout << std::setw(9) << ax << " ";
        std::cout << std::setw(9) << ay << " ";
        std::cout << std::setw(9) << az << " ";
        // std::cout << std::endl;
        std::cout << std::setw(9) << gx << " ";
        std::cout << std::setw(9) << gy << " ";
        std::cout << std::setw(9) << gz << " ";
        std::cout << std::endl;
    }
    return 0;
}