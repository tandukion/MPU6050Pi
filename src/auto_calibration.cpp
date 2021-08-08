#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <vector>
#include <sys/stat.h>

#include <stdio.h>
#include <bcm2835.h>
#include "I2Cdev.h"
#include "MPU6050.h"


MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int ax_mean, ay_mean, az_mean, gx_mean, gy_mean, gz_mean;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

float accel_sensitivity = 0;

float GetAccelSensitivity() {
    /*
    *  AFS_SEL  | Full Scale Range |   LSB Sensitivity
    *      0           2 g              16384 LSB/g
    *      1           4 g               8192 LSB/g
    *      2           8 g               4096 LSB/g
    *      3          16 g               2048 LSB/g
    */
    float LSB_sensitivity[] = {
        16384.0,
        8192.0,
        4096.0,
        2048.0,
    };

    // get AFS_SEL bit data
    uint8_t accel_fs;
    accel_fs = mpu.getFullScaleAccelRange();

    return LSB_sensitivity[accel_fs];
}

void GetAverage(int* ax_mean, int* ay_mean, int* az_mean, int* gx_mean, int* gy_mean, int* gz_mean) {
    int i;
    int buffer_size = 1000;

    int16_t ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
    int ax_buffer=0, ay_buffer=0, az_buffer=0, gx_buffer=0, gy_buffer=0, gz_buffer=0;

    for (i=0; i < buffer_size+100; i++) {
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        std::cout << "Read: ";
        std::cout << std::setw(9) << ax << std::setw(9) << ay << std::setw(9) << az;
        std::cout << std::setw(9) << gx << std::setw(9) << gy << std::setw(9) << gz;
        std::cout << "\r";

        if (i >=100) {
            ax_buffer += ax;
            ay_buffer += ay;
            az_buffer += az;
            gx_buffer += gx;
            gy_buffer += gy;
            gz_buffer += gz;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(2));
    }

    *ax_mean = ax_buffer/buffer_size;
    *ay_mean = ay_buffer/buffer_size;
    *az_mean = az_buffer/buffer_size;
    *gx_mean = gx_buffer/buffer_size;
    *gy_mean = gy_buffer/buffer_size;
    *gz_mean = gz_buffer/buffer_size;
}

void Calibrate(int* ax_mean, int* ay_mean, int* az_mean, int* gx_mean, int* gy_mean, int* gz_mean){  
    // Threshold for allower error. Reduce to reduce error, but will be more difficult to converge 
    int accel_error_threshold = 8;
    int gyro_error_threshold = 1;

    ax_offset = - *ax_mean/8;
    ay_offset = - *ay_mean/8;
    az_offset = - *az_mean/8;
    gx_offset = - *gx_mean/4;
    gy_offset = - *gy_mean/4;
    gz_offset = - *gz_mean/4;

    // Handle gravity axis (default when calibrating on flat position = Z-axis)
    az_offset += accel_sensitivity / 8;

    bool ready = false;
    int error;
    while (error>0) {
        error = 0;

        mpu.setXAccelOffset(ax_offset);
        mpu.setYAccelOffset(ay_offset);
        mpu.setZAccelOffset(az_offset);

        mpu.setXGyroOffset(gx_offset);
        mpu.setYGyroOffset(gy_offset);
        mpu.setZGyroOffset(gz_offset);

        GetAverage(&(*ax_mean), &(*ay_mean), &(*az_mean), &(*gx_mean), &(*gy_mean), &(*gz_mean));

        std::cout << "Mean: ";
        std::cout << std::setw(9) << *ax_mean << std::setw(9) << *ay_mean << std::setw(9) << *az_mean;
        std::cout << std::setw(9) << *gx_mean << std::setw(9) << *gy_mean << std::setw(9) << *gz_mean;
        std::cout << std::endl;

        // Correcting accelerometer offset
        if (abs(*ax_mean) > accel_error_threshold) {
            ax_offset -= *ax_mean / accel_error_threshold;
            error ++;
        }
        if (abs(*ay_mean) > accel_error_threshold) {
            ay_offset -= *ay_mean / accel_error_threshold;
            error ++;
        }
        // if (abs(*az_mean) > accel_error_threshold) {
        //     az_offset -= *az_mean / accel_error_threshold;
        //     error ++;
        // }
        // Need to retain +1g for gravity
        if (abs(*az_mean - accel_sensitivity) > accel_error_threshold) {
            az_offset -= *az_mean / accel_error_threshold;
            az_offset += accel_sensitivity / accel_error_threshold;
            error ++;
        }

        // Correcting gyroscope offset
        if (abs(*gx_mean) > gyro_error_threshold) {
            gx_offset -= *gx_mean / (gyro_error_threshold+1);
            error ++;
        }
        if (abs(*gy_mean) > gyro_error_threshold) {
            gy_offset -= *gy_mean / (gyro_error_threshold+1);
            error ++;
        }
        if (abs(*gz_mean) > gyro_error_threshold) {
            gz_offset -= *gz_mean / (gyro_error_threshold+1);
            error ++;
        }
    }
}

int main(int argc, char **argv) {
    std::cout << "Calibrating MPU6050." << std::endl;
    std::cout << "Please do this calibration with MPU6050 on flat surface (gravity on Z-axis)." << std::endl;
    std::cout << std::endl;

    // Check whether previously created calibration file exists
    std::string file_name = "calibration_file.csv";
    struct stat buf;
    if (stat(file_name.c_str(), &buf) == 0) {
        char u_input;
        int MAX_RETRY = 5;
        int try_count = 0;
        do {
            std::cout << "There is previously created calibration file. Replace with new calibration (y/n)? ";
            std::cin >> u_input;
            try_count++;
        }
        while(!std::cin.fail() && try_count < MAX_RETRY && u_input!='y' && u_input!='Y' && u_input!='n' && u_input!='N');

        if (try_count >= MAX_RETRY)
            return 0;

        if ((u_input == 'n') || (u_input == 'N'))
            return 0;
    }
    else {
        std::cout << "No existing calibration file. Creating a new one." << std::endl;
    }

    // Start I2C device
    I2Cdev::initialize();

    // Get accel sensitivity
    accel_sensitivity = GetAccelSensitivity();

    // Get the average from sensor reading
    std::cout << "\nGet the average from sensors" << std::endl;
    GetAverage(&ax_mean, &ay_mean, &az_mean, &gx_mean, &gy_mean, &gz_mean);

    std::cout << "Mean: ";
    std::cout << std::setw(9) << ax_mean << std::setw(9) << ay_mean << std::setw(9) << az_mean;
    std::cout << std::setw(9) << gx_mean << std::setw(9) << gy_mean << std::setw(9) << gz_mean;
    std::cout << std::endl;

    std::this_thread::sleep_for (std::chrono::seconds(1));

    // Calibration
    std::cout << "\nCalculating offset" << std::endl;
    Calibrate(&ax_mean, &ay_mean, &az_mean, &gx_mean, &gy_mean, &gz_mean);

    std::cout << "\nCalibration finished" << std::endl;
    std::cout << "Offset:" << std::endl;
    std::cout << std::setw(9) << ax_offset << std::setw(9) << ay_offset << std::setw(9) << az_offset;
    std::cout << std::setw(9) << gx_offset << std::setw(9) << gy_offset << std::setw(9) << gz_offset;
    std::cout << std::endl;

    // Saving to external file
    std::ofstream ofile;
    ofile.open(file_name);
    ofile << "AccelXOffset" << "," << "AccelYOffset" << "," << "AccelZOffset" << ",";
    ofile << "GyroXOffset" << "," << "GyroYOffset" << "," << "GyroZOffset" << std::endl;
    std::cout << std::setw(9) << ax_offset << std::setw(9) << ay_offset << std::setw(9) << az_offset;
    std::cout << std::setw(9) << gx_offset << std::setw(9) << gy_offset << std::setw(9) << gz_offset;
    std::cout << std::endl;
    ofile.close();

    return 0;
}