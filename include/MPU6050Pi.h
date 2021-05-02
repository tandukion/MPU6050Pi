#ifndef _MPU6050PI_H
#define _MPU6050PI_H

#include <cstdint>

#include "I2CPi.h"

#define G_FORCE 9.80665

/**
 * Register Map
 */
// Self test
#define SELF_TEST_X             0x0D
#define SELF_TEST_Y             0x0E
#define SELF_TEST_Z             0x0F
#define SELF_TEST_A             0x10

// Offset
#define XA_OFFS_H               0x06
#define XA_OFFS_L               0x07
#define YA_OFFS_H               0x08
#define YA_OFFS_L               0x09
#define ZA_OFFS_H               0x0A
#define ZA_OFFS_L               0x0B
#define XG_OFFSET_H             0x13
#define XG_OFFSET_L             0x14
#define YG_OFFSET_H             0x15
#define YG_OFFSET_L             0x16
#define ZG_OFFSET_H             0x17
#define ZG_OFFSET_L             0x18

// Config
#define SMPLRT_DIV              0x19    // Sample rate divider
#define CONFIG                  0x1A
#define GYRO_CONFIG             0x1B
#define ACCEL_CONFIG            0x1C
#define ACCEL_CONFIG_2          0x1D
#define LP_ACCEL_ODR            0x1E
#define WOM_THR                 0x1F

#define FIFO_EN                 0x23
#define I2C_MST_CTRL            0x24
#define I2C_SLV0_ADDR           0x25
#define I2C_SLV0_REG            0x26
#define I2C_SLV0_CTRL           0x27
#define I2C_SLV1_ADDR           0x28
#define I2C_SLV1_REG            0x29
#define I2C_SLV1_CTRL           0x2A
#define I2C_SLV2_ADDR           0x2B
#define I2C_SLV2_REG            0x2D
#define I2C_SLV2_CTRL           0x2C
#define I2C_SLV3_ADDR           0x2E
#define I2C_SLV3_REG            0x2F
#define I2C_SLV3_CTRL           0x30
#define I2C_SLV4_ADDR           0x31
#define I2C_SLV4_REG            0x32
#define I2C_SLV4_DO             0x33
#define I2C_SLV4_CTRL           0x34
#define I2C_SLV4_DI             0x35
#define I2C_MST_STATUS          0x36
#define INT_PIN_CFG             0x37
#define INT_ENABLE              0x38
#define INT_STATUS              0x3A

// Output
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40
#define TEMP_OUT_H              0x41
#define TEMP_OUT_L              0x42
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48

#define EXT_SENS_DATA_00        0x49
#define EXT_SENS_DATA_01        0x4A
#define EXT_SENS_DATA_02        0x4B
#define EXT_SENS_DATA_03        0x4C
#define EXT_SENS_DATA_04        0x4D
#define EXT_SENS_DATA_05        0x4E
#define EXT_SENS_DATA_06        0x4F
#define EXT_SENS_DATA_07        0x50
#define EXT_SENS_DATA_08        0x51
#define EXT_SENS_DATA_09        0x52
#define EXT_SENS_DATA_10        0x53
#define EXT_SENS_DATA_11        0x54
#define EXT_SENS_DATA_12        0x55
#define EXT_SENS_DATA_13        0x56
#define EXT_SENS_DATA_14        0x57
#define EXT_SENS_DATA_15        0x58
#define EXT_SENS_DATA_16        0x59
#define EXT_SENS_DATA_17        0x5A
#define EXT_SENS_DATA_18        0x5B
#define EXT_SENS_DATA_19        0x5C
#define EXT_SENS_DATA_20        0x5D
#define EXT_SENS_DATA_21        0x5E
#define EXT_SENS_DATA_22        0x5F
#define EXT_SENS_DATA_23        0x60

#define I2C_SLV0_DO             0x63
#define I2C_SLV1_DO             0x64
#define I2C_SLV2_DO             0x65
#define I2C_SLV3_DO             0x66

#define I2C_MST_DELAY_CTRL      0x67
#define SIGNAL_PATH_RESET       0x68
#define ACCEL_INTEL_CTRL        0x69
#define USER_CTRL               0x6A

#define PWR_MGMT_1              0x6B
#define PWR_MGMT_2              0x6C

#define FIFO_COUNT_H            0x72
#define FIFO_COUNT_L            0x73
#define FIFO_R_W                0x74
#define WHO_AM_I                0x75

/**
 * Parameter Settings
 */
#define FS_SEL_250              0x00
#define FS_SEL_500              0x01
#define FS_SEL_1000             0x02
#define FS_SEL_2000             0x03

#define AFS_SEL_2               0x00
#define AFS_SEL_4               0x01
#define AFS_SEL_8               0x02
#define AFS_SEL_16              0x03

#define DLPF_BW_260             0x00
#define DLPF_BW_184             0x01
#define DLPF_BW_94              0x02
#define DLPF_BW_44              0x03
#define DLPF_BW_21              0x04
#define DLPF_BW_10              0x05
#define DLPF_BW_5               0x06

/**
 * Class for 1 channel of L298 Motor Driver control using Raspberry Pi GPIO.
 */ 
class MPU6050Pi {
    private:
        uint8_t I2C_address_;
        int fd_;

        float gyro_sensitivity_;
        float accel_sensitivity_;

    public:
        /**
         * Constructor of the class
         * 
         * @param address   {int}   I2C address of the device. By default is 0x68.
         */
        MPU6050Pi(uint8_t address=0x68, uint8_t gyro_range=0, uint8_t accel_range=0);

        /**
         * Initialize the MPU6050 device.
         */
        void SetRate(uint8_t rate);

        void SetDLPFMode(uint8_t mode);

        void SetFullScaleGyroRange(uint8_t range=FS_SEL_250);
        void SetFullScaleAccelRange(uint8_t range=AFS_SEL_2);

        void SetXAccelOffset(int16_t offset);
        void SetYAccelOffset(int16_t offset);
        void SetZAccelOffset(int16_t offset);

        void SetXGyroOffset(int16_t offset);
        void SetYGyroOffset(int16_t offset);
        void SetZGyroOffset(int16_t offset);

        void GetMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

        void GetAccel(int16_t* ax, int16_t* ay, int16_t* az);
        void GetAccelX(int16_t* ax);
        void GetAccelY(int16_t* ay);
        void GetAccelZ(int16_t* az);
        void GetAccelFloat(float* ax, float* ay, float* az);

        void GetGyro(int16_t* gx, int16_t* gy, int16_t* gz);
        void GetGyroFloat(float* gx, float* gy, float* gz);

        float GetAccelSensitivity();
        float GetGyroSensitivity();
};

#endif