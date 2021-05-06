#include <iostream>	
#include <iomanip>
#include <fstream>
#include <string.h>


#include <MPU6050Pi.h>

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
    // Connect to device with default setting
    MPU6050Pi mpu;

    if (argc > 1){
        int16_t *offsets;

        offsets = GetCalibrationData(argv[1]);
        if (offsets){
            mpu.SetOffset(offsets);
        }
        else {
            std::cout << "Proceed with default offsets" << std::endl;
        }
    }

    int16_t ax, ay, az, gx, gy, gz;

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
        mpu.GetMotion6(&ax, &ay, &az, &gx, &gy, &gz);

        // 2. Get separated gyroscope and accelerometer
        // Get gyroscope data.
        // mpu.GetGyro(&gx, &gy, &gz);
        // Get accelerometer values.
        // mpu.GetAccel(&ax, &ay, &az);

        std::cout << std::setw(9) << ax << std::setw(9) << ay << std::setw(9) << az;
        std::cout << std::setw(9) << gx << std::setw(9) << gy << std::setw(9) << gz;
        std::cout << "\r";
    }
    return 0;
}