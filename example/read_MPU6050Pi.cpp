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
    float accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z;
    int devStatus;

    std::cout << "----------------------------" << std::endl;
    std::cout << "1. Raw Sensor Data (int)" << std::endl;
    std::cout << "2. Float Sensor Data" << std::endl;
    std::cout << "3. Orientation (Complementary filter)" << std::endl;
    std::cout << "4. DMP" << std::endl;
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
            break;
        case 2:
            // ====== Float Sensor Data ======
            std::cout << "Float Sensor Data\n";
            std::cout << std::fixed << std::setprecision(6) << std::setfill(' ');
            std::cout << std::setw(10) << "X(g)" << std::setw(10) << "Y(g)" << std::setw(10) << "Z(g)";
            std::cout << std::setw(10) << "X(deg/s)" << std::setw(10) << "Y(deg/s)" << std::setw(10) << "Z(deg/s)";
            std::cout << std::endl;

            // Publish in loop.
            while(1) {
                // Get gyroscope data.
                mpu.GetGyroFloat(&gyro_x, &gyro_y, &gyro_z);
                // Get accelerometer values.
                mpu.GetAccelFloat(&accel_x, &accel_y, &accel_z);

                std::cout << std::setw(10) << accel_x << std::setw(10) << accel_y << std::setw(10) << accel_z;
                std::cout << std::setw(10) << gyro_x << std::setw(10) << gyro_y << std::setw(10) << gyro_z;
                std::cout << "\r";
            }
            break;
        
        case 4:
            std::cout << "Initalize DMP..\n";
            devStatus = mpu.DMPInitalize();

            // make sure it worked (returns 0 if so)
            if (devStatus == 0) {
                // turn on the DMP, now that it's ready
                std::cout << "Enabling DMP...\n";
                mpu.SetDMPEnabled(true);

                // get expected DMP packet size for later comparison
                // packetSize = mpu.dmpGetFIFOPacketSize();
            } else {
                // ERROR!
                // 1 = initial memory load failed
                // 2 = DMP configuration updates failed
                // (if it's going to break, usually the code will be 1)
                std::cout << "DMP Initialization failed.\n Error code: ";
                std::cout << devStatus << "\n";
            } 

            break;

    }
    return 0;
}