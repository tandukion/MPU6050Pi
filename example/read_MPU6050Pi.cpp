#include <iostream>	
#include <iomanip>
#include <fstream>
#include <string.h>


#include "MPU6050.h"

/*
*  AFS_SEL  | Full Scale Range |   LSB Sensitivity
*      0           2 g              16384 LSB/g
*      1           4 g               8192 LSB/g
*      2           8 g               4096 LSB/g
*      3          16 g               2048 LSB/g
*/
float accel_lsb[] = {
    16384.0,
    8192.0,
    4096.0,
    2048.0,
};
/*
* Set Gyroscope FullScale Range
*  FS_SEL  | Full Scale Range  |   LSB Sensitivity
*      0           250 deg/s           131 LSB/deg/s
*      1           500 deg/s          65.5 LSB/deg/s
*      2          1000 deg/s          32.8 LSB/deg/s
*      3          2000 deg/s          16.4 LSB/deg/s
*/

float gyro_lsb[] = {
    131.0,
    65.5,
    32.8,
    16.4,
};

int16_t* GetCalibrationData (char *filename) {
    int16_t *offsets = new int16_t[6];

    // Getting the file extension
    char *file_ext = strrchr(filename, '.');
    if (file_ext) {
        // offsets = GetCalibrationData(argv[1]);
        // Make sure the calibration file is CSV
        if (strcmp(file_ext, ".csv") == 0) {
            std::ifstream ifile (filename);
            std::string header;
            std::string data_str;
            if (ifile.is_open()){
                // Getting data line by line
                // 1st line should be Header
                std::getline(ifile,header);

                // Data
                std::getline(ifile,data_str);

                ifile.close();

                // Parse csv
                int i=0;
                size_t pos = 0;
                std::string token;
                std::string delimiter = ",";
                while ((pos = data_str.find(delimiter)) != std::string::npos) {
                    offsets[i] = stoi(data_str.substr(0, pos));
                    data_str.erase(0, pos + delimiter.length());
                    i++;
                }
                // Handle last data if not ending with comma
                if (data_str.length() > 0){
                    offsets[i] = stoi(data_str);
                }
            }
            return offsets;
        }
        else {
            std::cout << "Wrong input file" << std::endl;
            return 0;
        }
    }
    else {
        std::cout << "Wrong input file" << std::endl;
        return 0;
    }
}

int main(int argc, char **argv) {
    // Start I2C device
    I2Cdev::initialize();
    
    // Connect to device with default setting
    MPU6050 mpu;

    if (argc > 1){
        int16_t *offsets;

        offsets = GetCalibrationData(argv[1]);
        if (offsets){
            // mpu.SetOffset(offsets);
            mpu.setXAccelOffset(offsets[0]);
            mpu.setYAccelOffset(offsets[1]);
            mpu.setZAccelOffset(offsets[2]);
            mpu.setXGyroOffset(offsets[3]);
            mpu.setYGyroOffset(offsets[4]);
            mpu.setZGyroOffset(offsets[5]);
        }
        else {
            std::cout << "Proceed with default offsets" << std::endl;
        }
    }

    int16_t ax, ay, az, gx, gy, gz;
    float accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z;

    std::cout << "----------------------------" << std::endl;
    std::cout << "1. Raw Sensor Data (int)" << std::endl;
    std::cout << "2. Float Sensor Data" << std::endl;
    std::cout << "3. Orientation (Complementary filter)" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Choose mode to choose: ";
    
    int mode;
    std::cin >> mode;

    switch(mode){
        case 1:
            // ====== Raw Sensor Data ======
            std::cout << "Raw Sensor Data\n";
            std::cout << std::fixed << std::setprecision(6) << std::setfill(' ');
            std::cout << std::setw(9) << 'X' << std::setw(9) << 'Y' << std::setw(9) << 'Z';
            std::cout << std::setw(9) << 'X' << std::setw(9) << 'Y' << std::setw(9) << 'Z';
            std::cout << std::endl;

            // Publish in loop.
            while(1) {
                // Choose between two methods here:
                // 1. Get from one single function for both accelerometer and gyroscope
                mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

                // 2. Get separated gyroscope and accelerometer
                // Get gyroscope data.
                // mpu.GetGyro(&gx, &gy, &gz);
                // Get accelerometer values.
                // mpu.GetAccel(&ax, &ay, &az);

                std::cout << std::setw(9) << ax << std::setw(9) << ay << std::setw(9) << az;
                std::cout << std::setw(9) << gx << std::setw(9) << gy << std::setw(9) << gz;
                std::cout << "\r";
            }
            break;
        case 2:
            // get AFS_SEL bit data
            uint8_t accel_fs = mpu.getFullScaleAccelRange();
            float accel_sensitivity = accel_lsb[accel_fs];
            uint8_t gyro_fs = mpu.getFullScaleGyroRange();
            float gyro_sensitivity = gyro_lsb[gyro_fs];
            
            // ====== Float Sensor Data ======
            std::cout << "Float Sensor Data\n";
            std::cout << std::fixed << std::setprecision(6) << std::setfill(' ');
            std::cout << std::setw(10) << "X(g)" << std::setw(10) << "Y(g)" << std::setw(10) << "Z(g)";
            std::cout << std::setw(10) << "X(deg/s)" << std::setw(10) << "Y(deg/s)" << std::setw(10) << "Z(deg/s)";
            std::cout << std::endl;

            // Publish in loop.
            while(1) {
                // // Get gyroscope data.
                // mpu.GetGyroFloat(&gyro_x, &gyro_y, &gyro_z);
                // // Get accelerometer values.
                // mpu.GetAccelFloat(&accel_x, &accel_y, &accel_z);

                mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
                accel_x = (float) ax / accel_sensitivity;
                accel_y = (float) ay / accel_sensitivity;
                accel_z = (float) az / accel_sensitivity;
                gyro_x = (float) gx / gyro_sensitivity;
                gyro_y = (float) gy / gyro_sensitivity;
                gyro_z = (float) gz / gyro_sensitivity;

                std::cout << std::setw(10) << accel_x << std::setw(10) << accel_y << std::setw(10) << accel_z;
                std::cout << std::setw(10) << gyro_x << std::setw(10) << gyro_y << std::setw(10) << gyro_z;
                std::cout << "\r";
            }
            break;

    }
    return 0;
}