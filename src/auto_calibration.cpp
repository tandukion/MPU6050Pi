#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>
#include <vector>

#include <MPU6050Pi.h>


MPU6050Pi mpu;
int16_t ax, ay, az, gx, gy, gz;
int ax_mean, ay_mean, az_mean, gx_mean, gy_mean, gz_mean;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;

void GetAverage(int* ax_mean, int* ay_mean, int* az_mean, int* gx_mean, int* gy_mean, int* gz_mean) {
    int i;
    int buffer_size = 1000;

    int16_t ax=0, ay=0, az=0, gx=0, gy=0, gz=0;
    int ax_buffer=0, ay_buffer=0, az_buffer=0, gx_buffer=0, gy_buffer=0, gz_buffer=0;

    for (i=0; i < buffer_size+100; i++) {
        mpu.GetMotion6(&ax, &ay, &az, &gx, &gy, &gz);

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
    int accel_error_threshold = 8;
    int gyro_error_threshold = 1;

    ax_offset = - *ax_mean/8;
    ay_offset = - *ay_mean/8;
    az_offset = - *az_mean/8;
    gx_offset = - *gx_mean/4;
    gy_offset = - *gy_mean/4;
    gz_offset = - *gz_mean/4;

    // Check which axis on gravity axis
    std::vector<int> accel{abs(ax_offset), abs(ay_offset), abs(az_offset)};
    std::vector<int>::iterator max = std::max_element(accel.begin(), accel.end());
    int g_axis = std::distance(accel.begin(), max);
    switch (g_axis) {
        case 0:
            ax_offset += mpu.GetAccelSensitivity() / 8;
            break;
        case 1:
            ay_offset += mpu.GetAccelSensitivity() / 8;
            break;
        case 2:
            az_offset += mpu.GetAccelSensitivity() / 8;
            break;
    }

    bool ready = false;
    int error;
    while (error>0) {
        error = 0;

        mpu.SetXAccelOffset(ax_offset);
        mpu.SetYAccelOffset(ay_offset);
        mpu.SetZAccelOffset(az_offset);

        mpu.SetXGyroOffset(gx_offset);
        mpu.SetYGyroOffset(gy_offset);
        mpu.SetZGyroOffset(gz_offset);

        GetAverage(&(*ax_mean), &(*ay_mean), &(*az_mean), &(*gx_mean), &(*gy_mean), &(*gz_mean));

        std::cout << *ax_mean << " " << *ay_mean << " " << *az_mean << " " << *gx_mean << " " << *gy_mean << " " << *gz_mean << std::endl;

        // Correcting accelerometer offset
        if (abs(*ax_mean) > accel_error_threshold) {
            ax_offset -= *ax_mean / accel_error_threshold;
            error ++;
        }
        if (abs(*ay_mean) > accel_error_threshold) {
            ay_offset -= *ay_mean / accel_error_threshold;
            error ++;
        }
        if (abs(*az_mean) > accel_error_threshold) {
            az_offset -= *az_mean / accel_error_threshold;
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
    // Get the average from sensor reading
    std::cout << "Get the average from sensors" << std::endl;
    GetAverage(&ax_mean, &ay_mean, &az_mean, &gx_mean, &gy_mean, &gz_mean);

    std::cout << "Mean" << std::endl;
    std::cout << ax_mean << " " << ay_mean << " " << az_mean << " " << gx_mean << " " << gy_mean << " " << gz_mean << std::endl;

    std::this_thread::sleep_for (std::chrono::seconds(1));

    // Calibration
    std::cout << "Calculating offset" << std::endl;
    Calibrate(&ax_mean, &ay_mean, &az_mean, &gx_mean, &gy_mean, &gz_mean);

    std::cout << "Finished" << std::endl;
    std::cout << "Mean" << std::endl;
    std::cout << ax_mean << " " << ay_mean << " " << az_mean << " " << gx_mean << " " << gy_mean << " " << gz_mean << std::endl;
    std::cout << "Offset" << std::endl;
    std::cout << ax_offset << " " << ay_offset << " " << az_offset << " " << gx_offset << " " << gy_offset << " " << gz_offset << std::endl;

    return 0;
}