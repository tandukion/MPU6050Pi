
#ifndef _MPU6050PI_MOTIONAPPS20_H_
#define _MPU6050PI_MOTIONAPPS20_H_


#define DMP_CODE_SIZE           1929    // dmpMemory[]
#define DMP_CONFIG_SIZE         192     // dmpConfig[]
#define DMP_UPDATES_SIZE        47      // dmpUpdates[]


#define DMP_MEMORY_BANKS        8
#define DMP_MEMORY_BANK_SIZE    256
#define DMP_MEMORY_CHUNK_SIZE   16

#define DMP_FIFO_RATE_DIVISOR   0x01 

class DMP {
    public:
        static const unsigned char memory[];
};

#endif