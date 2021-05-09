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
 * Bit selector
 */
#define CONFIG_DLPF_CFG_START       0
#define CONFIG_DLPF_CFG_LENGTH      3

#define PWR_MGMT_1_CLKSEL_START     0
#define PWR_MGMT_1_CLKSEL_LENGTH    3
#define PWR_MGMT_1_TEMP_DIS_BIT     3
#define PWR_MGMT_1_CYCLE_BIT        5
#define PWR_MGMT_1_SLEEP_BIT        6
#define PWR_MGMT_1_RESET_BIT        7

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

#define CLOCK_INTERNAL          0x00
#define CLOCK_PLL_XGYRO         0x01
#define CLOCK_PLL_YGYRO         0x02
#define CLOCK_PLL_ZGYRO         0x03
#define CLOCK_PLL_EXT32K        0x04
#define CLOCK_PLL_EXT19M        0x05
#define CLOCK_KEEP_RESET        0x07

#define SLEEP_DISABLED          0
#define SLEEP_ENABLED           1

/**
 * Class for MPU6050 sensor reading using Raspberry Pi GPIO.
 */ 
class MPU6050Pi {
    private:
        uint8_t I2C_address_;
        int fd_;    // I2C device file handler

        float gyro_sensitivity_;
        float accel_sensitivity_;
        float gyro_rate_;
        float sample_rate_;

    public:
        /** ============================================================
         *      CONSTRUCTOR
         *  ============================================================
         */
        /**
         * Constructor of the class using all default settings
         */
        MPU6050Pi();
        /**
         * Constructor of the class
         * 
         * @param offsets {int16_t*} Array of offsets: {AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ}
         */
        MPU6050Pi(int16_t *offsets);

        /** ============================================================
         *      CONFIGURATION
         *  ============================================================
         */
        /**
         * Set Sample Rate Divider (SMPLRT_DIV) for Gyroscope Output Rate.
         * 
         * @param rate {uint8_t} Sample rate divider 
         */
        void SetSampleRateDivider(uint8_t rate);
        /**
         * Return current sample rate
         */
        float GetSampleRate();

        /**
         * Set Digital Low Pass Filter (DLPF) mode. See register map datasheet for DLPF_CFG.
         * 
         * @param mode {uint8_t} Value for DLPF_CFG
         */
        void SetDLPFMode(uint8_t mode);
        /**
         * Return the current DLPF_CFG from CONFIG register
         */
        uint8_t GetDLPFMode();

        /**
         * Set Gyroscope FullScale Range
         *  FS_SEL  | Full Scale Range  |   LSB Sensitivity
         *      0           250 deg/s           131 LSB/deg/s
         *      1           500 deg/s          65.5 LSB/deg/s
         *      2          1000 deg/s          32.8 LSB/deg/s
         *      3          2000 deg/s          16.4 LSB/deg/s
         * 
         * @param range {uint8_t} Value for FS_SEL. Default to 0.
         */
        void SetFullScaleGyroRange(uint8_t range=FS_SEL_250);
        /**
         * Return current gyroscope sensitivity per Least Significant Bit (LSB)
         */
        float GetGyroSensitivity();

        /**
         * Set Accelerometer FullScale Range
         *  AFS_SEL  | Full Scale Range |   LSB Sensitivity
         *      0           2 g              16384 LSB/g
         *      1           4 g               8192 LSB/g
         *      2           8 g               4096 LSB/g
         *      3          16 g               2048 LSB/g
         * 
         * @param range {uint8_t} Value for AFS_SEL. Default to 0.
         */
        void SetFullScaleAccelRange(uint8_t range=AFS_SEL_2);
        /**
         * Return current accerelerometer sensitivity per Least Significant Bit (LSB)
         */
        float GetAccelSensitivity();

        /**
         * Set all offsets for accelerometer and gyroscope
         * 
         * @param offset {int16_t*} Array of offsets: {AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ}
         */
        void SetOffset(int16_t *offset);
        /**
         * Set all offsets for accelerometer
         * 
         * @param offset {int16_t*} Array of offsets: {AccelX, AccelY, AccelZ}
         */
        void SetAccelOffset(int16_t *offset);
        /**
         * Set X-axis offsets of accelerometer
         * 
         * @param offset {int16_t} offset
         */
        void SetAccelXOffset(int16_t offset);
        /**
         * Set Y-axis offsets of accelerometer
         * 
         * @param offset {int16_t} offset
         */
        void SetAccelYOffset(int16_t offset);
        /**
         * Set Z-axis offsets of accelerometer
         * 
         * @param offset {int16_t} offset
         */
        void SetAccelZOffset(int16_t offset);

        /**
         * Set all offsets for gyroscope
         * 
         * @param offset {int16_t*} Array of offsets: {GyroX, GyroY, GyroZ}
         */
        void SetGyroOffset(int16_t *offset);
        /**
         * Set X-axis offsets of gyroscope
         * 
         * @param offset {int16_t} offset
         */
        void SetGyroXOffset(int16_t offset);
        /**
         * Set Y-axis offsets of gyroscope
         * 
         * @param offset {int16_t} offset
         */
        void SetGyroYOffset(int16_t offset);
        /**
         * Set Z-axis offsets of gyroscope
         * 
         * @param offset {int16_t} offset
         */
        void SetGyroZOffset(int16_t offset);

        /**
         * Set Clock source. Upon pwer up, by default CLK_SEL=0 for internal oscillator.
         * CLK_SEL  |   Clock Source
        *     0     |   Internal oscillator
        *     1     |   PLL with X Gyro reference
        *     2     |   PLL with Y Gyro reference
        *     3     |   PLL with Z Gyro reference
        *     4     |   PLL with external 32.768kHz reference
        *     5     |   PLL with external 19.2MHz reference
        *     6     |   Reserved
        *     7     |   Stops the clock and keeps the timing generator in reset
         * 
         * @param offset {int16_t} offset
         */
        void SetClockSource(uint8_t clk_sel);

        /**
         * Set Sleep mode
         * 
         * @param mode {uint8_t} 0: disabled, 1: sleep mode
         */
        void SetSleepMode(uint8_t mode);

        /** ============================================================
         *      DATA
         *  ============================================================
         */
        /**
         * Return 16bit signed raw sensor data for accelerometer and gyroscope
         * 
         * @param ax {int16_t} accelerometer X-axis
         * @param ay {int16_t} accelerometer Y-axis
         * @param az {int16_t} accelerometer Z-axis
         * @param gx {int16_t} gyroscope X-axis
         * @param gy {int16_t} gyroscope Y-axis
         * @param gz {int16_t} gyroscope Z-axis
         */
        void GetMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);

        /**
         * Return 16bit signed raw sensor data for accelerometer
         * 
         * @param ax {int16_t} accelerometer X-axis
         * @param ay {int16_t} accelerometer Y-axis
         * @param az {int16_t} accelerometer Z-axis
         */
        void GetAccel(int16_t* ax, int16_t* ay, int16_t* az);
        /**
         * Return 16bit signed raw sensor data for accelerometer X-axis
         * 
         * @param ax {int16_t} data
         */
        void GetAccelX(int16_t* ax);
        /**
         * Return 16bit signed raw sensor data for accelerometer Y-axis
         * 
         * @param ay {int16_t} data
         */
        void GetAccelY(int16_t* ay);
        /**
         * Return 16bit signed raw sensor data for accelerometer Z-axis
         * 
         * @param az {int16_t} data
         */
        void GetAccelZ(int16_t* az);
        /**
         * Return float-converted sensor data for accelerometer in g (gravity)
         * 
         * @param ax {float} accelerometer X-axis
         * @param ay {float} accelerometer Y-axis
         * @param az {float} accelerometer Z-axis
         */
        void GetAccelFloat(float* ax, float* ay, float* az);

        /**
         * Return 16bit signed raw sensor data for gyroscope
         * 
         * @param gx {int16_t} gyroscope X-axis
         * @param gy {int16_t} gyroscope Y-axis
         * @param gz {int16_t} gyroscope Z-axis
         */
        void GetGyro(int16_t* gx, int16_t* gy, int16_t* gz);
        /**
         * Return float-converted sensor data for gyroscope in deg/s
         * 
         * @param gx {float} gyroscope X-axis
         * @param gy {float} gyroscope Y-axis
         * @param gz {float} gyroscope Z-axis
         */
        void GetGyroFloat(float* gx, float* gy, float* gz);

};

#endif