#include "MPU6050Pi.h"

/** ============================================================
 *      CONSTRUCTOR
 *  ============================================================
 */
MPU6050Pi::MPU6050Pi() {
    // Set default the I2C address of the device
    I2C_address_ = 0x68;

    // Initialize the I2C device file handler
    fd_ = I2CPi::Setup(0x68);

    // Set Clock Source. Better to use X gyro as reference
    MPU6050Pi::SetClockSource(CLOCK_PLL_XGYRO);

    // Disable SLEEP mode
    MPU6050Pi::SetSleepMode(SLEEP_DISABLED);

    // Set DLPF (Digital Low Pass Filter) to 44Hz. Check table in Register Map.
    MPU6050Pi::SetDLPFMode(DLPF_BW_44);

    // Set sample rate divider to 200Hz, div=5.
    MPU6050Pi::SetSampleRateDivider(0x05);

    // Configure gyroscope setting with default range
    MPU6050Pi::SetFullScaleGyroRange(FS_SEL_250);

    // Configure accelerometer setting with default range
    MPU6050Pi::SetFullScaleAccelRange(AFS_SEL_2);

    // Set offsets with default zeros
    MPU6050Pi::SetAccelXOffset(0x0000);
    MPU6050Pi::SetAccelYOffset(0x0000);
    MPU6050Pi::SetAccelZOffset(0x0000);
    MPU6050Pi::SetGyroXOffset(0x0000);
    MPU6050Pi::SetGyroYOffset(0x0000);
    MPU6050Pi::SetGyroZOffset(0x0000);
}

MPU6050Pi::MPU6050Pi(int16_t *offsets) {
    // Set default the I2C address of the device
    I2C_address_ = 0x68;

    // Initialize the I2C device file handler
    fd_ = I2CPi::Setup(0x68);

    // Set Clock Source. Better to use X gyro as reference
    MPU6050Pi::SetClockSource(CLOCK_PLL_XGYRO);

    // Disable SLEEP mode
    MPU6050Pi::SetSleepMode(SLEEP_DISABLED);

    // Set DLPF (Digital Low Pass Filter) to 44Hz. Check table in Register Map.
    MPU6050Pi::SetDLPFMode(DLPF_BW_44);

    // Set sample rate divider to 200Hz, div=5.
    MPU6050Pi::SetSampleRateDivider(0x05);

    // Configure gyroscope setting with default range
    MPU6050Pi::SetFullScaleGyroRange(FS_SEL_250);

    // Configure accelerometer setting with default range
    MPU6050Pi::SetFullScaleAccelRange(AFS_SEL_2);

    // Set offsets
    MPU6050Pi::SetAccelXOffset(offsets[0]);
    MPU6050Pi::SetAccelYOffset(offsets[1]);
    MPU6050Pi::SetAccelZOffset(offsets[2]);
    MPU6050Pi::SetGyroXOffset(offsets[4]);
    MPU6050Pi::SetGyroYOffset(offsets[5]);
    MPU6050Pi::SetGyroZOffset(offsets[6]);
}

/** ============================================================
 *      CONFIGURATION
 *  ============================================================
 */
void MPU6050Pi::SetSampleRateDivider(uint8_t rate) {
    I2CPi::WriteByte(fd_, SMPLRT_DIV, rate);

    sample_rate_ = gyro_rate_/(1+rate);
}

float MPU6050Pi::GetSampleRate() {
    return sample_rate_;
}

// ---------- CONFIG registers ----------
void MPU6050Pi::SetExternalFrameSync(uint8_t sync) {
    I2CPi::WriteBits(fd_, CONFIG, EXT_SYNC_SET_START, EXT_SYNC_SET_LENGTH, sync);
}

void MPU6050Pi::SetDLPFMode(uint8_t mode) {
    // Set sample rate based on DLPF_CFG
    if ((mode == DLPF_BW_260) || (mode > DLPF_BW_5)){
        gyro_rate_ = 8; // 8 kHz
    }
    else {
        gyro_rate_ = 1; // 1 kHz
    }

    // Set only the DLPF_CFG on bit 0,1,2
    I2CPi::WriteBits(fd_, CONFIG, CONFIG_DLPF_CFG_START, CONFIG_DLPF_CFG_LENGTH, mode);
}

// ---------- GYRO_CONFIG registers ----------
void MPU6050Pi::SetFullScaleGyroRange(uint8_t range) {
    uint8_t gyro_config_val;
    switch (range) {
        case 0:         // 250 deg/s full scale range
            gyro_sensitivity_ = 131.0;
            gyro_config_val = 0x00;
            break;
        case 1:         // 500 deg/s full scale range
            gyro_sensitivity_ = 65.5;
            gyro_config_val = 0x08;
            break;
        case 2:         // 1000 deg/s full scale range
            gyro_sensitivity_ = 32.8;
            gyro_config_val = 0x10;
            break;
        case 3:         // 2000 deg/s full scale range
            gyro_sensitivity_ = 16.4;
            gyro_config_val = 0x18;
            break;
    }
    I2CPi::WriteByte(fd_, GYRO_CONFIG, gyro_config_val);
}

float MPU6050Pi::GetGyroSensitivity() {
    return gyro_sensitivity_;
}

// ---------- ACCEL_CONFIG registers ----------
void MPU6050Pi::SetFullScaleAccelRange(uint8_t range) {
    uint8_t accel_config_val;
    switch (range) {
        case 0x00:      // 250 deg/s full scale range
            accel_sensitivity_ = 16384.0;
            accel_config_val = 0x00;
            break;
        case 0x01:      // 500 deg/s full scale range
            accel_sensitivity_ = 8192.0;
            accel_config_val = 0x08;
            break;
        case 0x02:      // 1000 deg/s full scale range
            accel_sensitivity_ = 4096.0;
            accel_config_val = 0x10;
            break;
        case 0x03:      // 2000 deg/s full scale range
            accel_sensitivity_ = 2048.0;
            accel_config_val = 0x18;
            break;
    }
    I2CPi::WriteByte(fd_, ACCEL_CONFIG, accel_config_val);
}

float MPU6050Pi::GetAccelSensitivity() {
    return accel_sensitivity_;
}

// ---------- FF* registers ----------
void MPU6050Pi::SetFreefallDetectionThreshold(uint8_t threshold){
    I2CPi::WriteByte(fd_, FF_THR, threshold);
}
uint8_t MPU6050Pi::GetFreefallDetectionThreshold(){
    return I2CPi::ReadByte(fd_, FF_THR);
}

void MPU6050Pi::SetFreefallDetectionDuration(uint8_t duration){
    I2CPi::WriteByte(fd_, FF_DUR, duration);
}
uint8_t MPU6050Pi::GetFreefallDetectionDuration(){
    return I2CPi::ReadByte(fd_, FF_DUR);
}

// ---------- MOT* registers ----------
void MPU6050Pi::SetMotionDetectionThreshold(uint8_t threshold){
    I2CPi::WriteByte(fd_, MOT_THR, threshold);
}
uint8_t MPU6050Pi::GetMotionDetectionThreshold(){
    return I2CPi::ReadByte(fd_, MOT_THR);
}

void MPU6050Pi::SetMotionDetectionDuration(uint8_t duration){
    I2CPi::WriteByte(fd_, MOT_DUR, duration);
}
uint8_t MPU6050Pi::GetMotionDetectionDuration(){
    return I2CPi::ReadByte(fd_, MOT_DUR);
}

// ---------- ZRMOT* registers ----------
void MPU6050Pi::SetZeroMotionDetectionThreshold(uint8_t threshold){
    I2CPi::WriteByte(fd_, ZRMOT_THR, threshold);
}
uint8_t MPU6050Pi::GetZeroMotionDetectionThreshold(){
    return I2CPi::ReadByte(fd_, ZRMOT_THR);
}

void MPU6050Pi::SetZeroMotionDetectionDuration(uint8_t duration){
    I2CPi::WriteByte(fd_, ZRMOT_DUR, duration);
}
uint8_t MPU6050Pi::GetZeroMotionDetectionDuration(){
    return I2CPi::ReadByte(fd_, ZRMOT_DUR);
}

// ---------- I2C_SLV* registers ----------
void MPU6050Pi::SetSlaveAddress(uint8_t num, uint8_t address) {
    if (num > 3) return;
    I2CPi::ReadByte(fd_, I2C_SLV0_ADDR + num*3);
}

uint8_t MPU6050Pi::GetSlaveAddress(uint8_t num) {
    if (num > 3) return 0;
    return I2CPi::ReadByte(fd_, I2C_SLV0_ADDR + num*3);
}

// ---------- INT_ENABLE registers ----------
void MPU6050Pi::SetIntEnabled(uint8_t enabled) {
    I2CPi::WriteByte(fd_, INT_ENABLE, enabled);
}
uint8_t MPU6050Pi::GetIntEnabled(){
    return I2CPi::ReadByte(fd_, INT_ENABLE);
}

// ---------- INT_STATUS registers ----------
uint8_t MPU6050Pi::GetIntStatus() {
    return I2CPi::ReadByte(fd_, INT_STATUS);
}

// ---------- *OFFS* registers ----------
void MPU6050Pi::SetOffset(int16_t *offset){
    I2CPi::WriteWord(fd_, XA_OFFS_H, offset[0]);
    I2CPi::WriteWord(fd_, YA_OFFS_H, offset[1]);
    I2CPi::WriteWord(fd_, ZA_OFFS_H, offset[2]);
    I2CPi::WriteWord(fd_, XG_OFFSET_H, offset[3]);
    I2CPi::WriteWord(fd_, YG_OFFSET_H, offset[4]);
    I2CPi::WriteWord(fd_, ZG_OFFSET_H, offset[5]);
}
void MPU6050Pi::SetAccelOffset(int16_t *offset){
    I2CPi::WriteWord(fd_, XA_OFFS_H, offset[0]);
    I2CPi::WriteWord(fd_, YA_OFFS_H, offset[1]);
    I2CPi::WriteWord(fd_, ZA_OFFS_H, offset[2]);
}
void MPU6050Pi::SetAccelXOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, XA_OFFS_H, offset);
}
void MPU6050Pi::SetAccelYOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, YA_OFFS_H, offset);
}
void MPU6050Pi::SetAccelZOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, ZA_OFFS_H, offset);
}

void MPU6050Pi::SetGyroOffset(int16_t *offset){
    I2CPi::WriteWord(fd_, XG_OFFSET_H, offset[0]);
    I2CPi::WriteWord(fd_, YG_OFFSET_H, offset[1]);
    I2CPi::WriteWord(fd_, ZG_OFFSET_H, offset[2]);
}
void MPU6050Pi::SetGyroXOffset(int16_t offset){
    I2CPi::WriteWord(fd_, XG_OFFSET_H, offset);
}
void MPU6050Pi::SetGyroYOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, YG_OFFSET_H, offset);
}
void MPU6050Pi::SetGyroZOffset(int16_t offset) {
    I2CPi::WriteWord(fd_, ZG_OFFSET_H, offset);
}

// ---------- USER_CTRL registers ----------

void MPU6050Pi::SetFIFOEnabled(bool enabled){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_FIFO_EN_BIT, enabled);
}

void MPU6050Pi::SetI2CMasterModeEnabled(bool enabled){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_I2C_MST_EN_BIT, enabled);
}

void MPU6050Pi::ResetFIFO(){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_FIFO_RESET_BIT, true);
}
void MPU6050Pi::ResetI2CMaster(){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_I2C_MST_RESET_BIT, true);
}
void MPU6050Pi::ResetSensors(){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_SIG_COND_RESET_BIT, true);
}

// ---------- PWR_MGMT_1 registers ----------
void MPU6050Pi::SetClockSource(uint8_t clk_sel) {
    // Set only the PWR_MGMT_1 on bit 0,1,2
    I2CPi::WriteBits(fd_, PWR_MGMT_1, PWR_MGMT_1_CLKSEL_START, PWR_MGMT_1_CLKSEL_LENGTH, clk_sel);
}

void MPU6050Pi::SetSleepMode(uint8_t mode){
    // Set only the PWR_MGMT_1 on bit PWR_MGMT_1_SLEEP_BIT
    I2CPi::WriteBit(fd_, PWR_MGMT_1, PWR_MGMT_1_SLEEP_BIT, mode);
}

void MPU6050Pi::ResetDevice(){
    // Set only the PWR_MGMT_1 on bit PWR_MGMT_1_SLEEP_BIT
    I2CPi::WriteBit(fd_, PWR_MGMT_1, PWR_MGMT_1_RESET_BIT, 1);
}

/** ============================================================
 *      DATA
 *  ============================================================
 */
// ---------- *OUT* registers ----------
void MPU6050Pi::GetMotion6(int16_t* ax, int16_t* ay, int16_t* az,
                           int16_t* gx, int16_t* gy, int16_t* gz) {
    *ax = I2CPi::ReadWord(fd_, ACCEL_XOUT_H);
    *ay = I2CPi::ReadWord(fd_, ACCEL_YOUT_H);
    *az = I2CPi::ReadWord(fd_, ACCEL_ZOUT_H);
    *gx = I2CPi::ReadWord(fd_, GYRO_XOUT_H);
    *gy = I2CPi::ReadWord(fd_, GYRO_YOUT_H);
    *gz = I2CPi::ReadWord(fd_, GYRO_ZOUT_H);
}

void MPU6050Pi::GetAccel(int16_t* ax, int16_t* ay, int16_t* az) {
    *ax = I2CPi::ReadWord(fd_, ACCEL_XOUT_H);
    *ay = I2CPi::ReadWord(fd_, ACCEL_YOUT_H);
    *az = I2CPi::ReadWord(fd_, ACCEL_ZOUT_H);
}

void MPU6050Pi::GetAccelY(int16_t* ay) {
    *ay = I2CPi::ReadWord(fd_, ACCEL_YOUT_H);
}

void MPU6050Pi::GetAccelFloat(float* ax, float* ay, float* az) {
    int16_t x, y, z;
    MPU6050Pi::GetAccel(&x, &y, &z);
    *ax = (float) x / accel_sensitivity_;
    *ay = (float) y / accel_sensitivity_;
    *az = (float) z / accel_sensitivity_;
}

void MPU6050Pi::GetGyro(int16_t* gx, int16_t* gy, int16_t* gz) {
    *gx = I2CPi::ReadWord(fd_, GYRO_XOUT_H);
    *gy = I2CPi::ReadWord(fd_, GYRO_YOUT_H);
    *gz = I2CPi::ReadWord(fd_, GYRO_ZOUT_H);
}

void MPU6050Pi::GetGyroFloat(float* gx, float* gy, float* gz) {
    int16_t x, y, z;
    MPU6050Pi::GetGyro(&x, &y, &z);
    *gx = (float) x / gyro_sensitivity_;
    *gy = (float) y / gyro_sensitivity_;
    *gz = (float) z / gyro_sensitivity_;
}

/** ============================================================
 *      UNDOCUMENTED DMP METHODS
 *  ============================================================
 */

void MPU6050Pi::SetDMPEnabled(bool enabled){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_DMP_EN_BIT, enabled);
}
void MPU6050Pi::ResetDMP(){
    I2CPi::WriteBit(fd_, USER_CTRL, USERCTRL_DMP_RESET_BIT, true);
}

// ---------- XG_OFFS_TC registers ----------
void MPU6050Pi::SetOTPBankValid(bool enabled){
    I2CPi::WriteBit(fd_, XG_OFFS_TC, TC_OTP_BNK_VLD_BIT, enabled);
}

uint8_t MPU6050Pi::GetOTPBankValid(){
    return I2CPi::ReadBit(fd_, XG_OFFS_TC, TC_OTP_BNK_VLD_BIT);
}

// ---------- BANK_SEL registers ----------
void MPU6050Pi::SetMemoryBank(uint8_t bank, bool prefetch_enabled, bool user_bank){
    bank &= 0x1F;
    if (user_bank) bank |= 0x20;
    if (prefetch_enabled) bank |= 0x40;
    I2CPi::WriteByte(fd_, BANK_SEL, bank);
}

// ---------- MEM_START_ADDR registers ----------
void MPU6050Pi::SetMemoryStartAddress(uint8_t address){
    I2CPi::WriteByte(fd_, MEM_START_ADDR, address);
}

// ---------- MEM_R_W registers ----------
void MPU6050Pi::WriteMemoryByte(uint8_t data){
    I2CPi::WriteByte(fd_, MEM_R_W, data);
}

uint8_t MPU6050Pi::ReadMemoryByte(){
    return I2CPi::ReadByte(fd_, MEM_R_W);
}

bool MPU6050Pi::WriteMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify, bool useProgMem){
    MPU6050Pi::SetMemoryBank(bank);
    MPU6050Pi::SetMemoryStartAddress(address);
    uint8_t chunkSize;
    uint8_t *verifyBuffer=0;
    uint8_t *progBuffer=0;
    uint16_t i;
    uint8_t j;
    if (verify) verifyBuffer = (uint8_t *)malloc(DMP_MEMORY_CHUNK_SIZE);
    if (useProgMem) progBuffer = (uint8_t *)malloc(DMP_MEMORY_CHUNK_SIZE);
    for (i = 0; i < dataSize;) {
        // determine correct chunk size according to bank position and data size
        chunkSize = DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) chunkSize = 256 - address;
        
        if (useProgMem) {
            // write the chunk of data as specified
            // for (j = 0; j < chunkSize; j++) progBuffer[j] = pgm_read_byte(data + i + j);
        } else {
            // write the chunk of data as specified
            progBuffer = (uint8_t *)data + i;
        }

        // I2CPi::WriteBytes(fd_, MEM_R_W, chunkSize, progBuffer);

        // verify data if needed
        if (verify && verifyBuffer) {
            MPU6050Pi::SetMemoryBank(bank);
            MPU6050Pi::SetMemoryStartAddress(address);
            // I2CPi::ReadBytes(fd_, MEM_R_W, chunkSize, verifyBuffer);
            if (memcmp(progBuffer, verifyBuffer, chunkSize) != 0) {
                free(verifyBuffer);
                if (useProgMem) free(progBuffer);
                return false; // uh oh.
            }
        }

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) {
            if (address == 0) bank++;
            MPU6050Pi::SetMemoryBank(bank);
            MPU6050Pi::SetMemoryStartAddress(address);
        }
    }
    if (verify) free(verifyBuffer);
    if (useProgMem) free(progBuffer);
    return true;

}

bool MPU6050Pi::WriteProgMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address, bool verify){
    return MPU6050Pi::WriteMemoryBlock(data, dataSize, bank, address, verify, true);
}

void MPU6050Pi::ReadMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank, uint8_t address){
    MPU6050Pi::SetMemoryBank(bank);
    MPU6050Pi::SetMemoryStartAddress(address);
    uint8_t chunkSize;
    for (uint16_t i = 0; i < dataSize;) {
        // determine correct chunk size according to bank position and data size
        chunkSize = DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) chunkSize = 256 - address;

        // read the chunk of data as specified
        // data = I2CPi::ReadBytes(fd_, MEM_R_W, chunkSize);
        
        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_t automatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) {
            if (address == 0) bank++;
            MPU6050Pi::SetMemoryBank(bank);
            MPU6050Pi::SetMemoryStartAddress(address);
        }
    }
}

// ---------- DMP_CFG* registers ----------
void MPU6050Pi::SetDMPConfig1(uint8_t config) {
    I2CPi::WriteByte(fd_, DMP_CFG_1, config);
}

uint8_t MPU6050Pi::GetDMPConfig1() {
    return I2CPi::ReadByte(fd_, DMP_CFG_1);
}

void MPU6050Pi::SetDMPConfig2(uint8_t config) {
    I2CPi::WriteByte(fd_, DMP_CFG_2, config);
}

uint8_t MPU6050Pi::GetDMPConfig2() {
    return I2CPi::ReadByte(fd_, DMP_CFG_2);
}

/** ============================================================
 *      MOTION PROCESSING
 *  ============================================================
 */
uint8_t MPU6050Pi::DMPInitalize() {
    // Reset device
    MPU6050Pi::ResetDevice();
    std::this_thread::sleep_for (std::chrono::milliseconds(30));

    MPU6050Pi::SetSleepMode(SLEEP_DISABLED);

    // Check Hardware Revision
	MPU6050Pi::SetMemoryBank(0x10, true, true);
	MPU6050Pi::SetMemoryStartAddress(0x06);
	std::cout << "Checking hardware revision... ";
	std::cout << "Revision @ user[16][6] = " << (int)MPU6050Pi::ReadMemoryByte() << std::endl;
	MPU6050Pi::SetMemoryBank(0, false, false);

	// check OTP bank valid
	std::cout << "Reading OTP bank valid flag... ";
	std::cout << "OTP bank is ";
    if (MPU6050Pi::GetOTPBankValid())
        std::cout << "valid." << std::endl;
    else
        std::cout << "invalid." << std::endl;

	// Set Slave
	std::cout << "Setting slave 0 address to 0x7F..." << std::endl;
	MPU6050Pi::SetSlaveAddress(0, 0x7F);
	std::cout << "Disabling I2C Master mode..." << std::endl;
	MPU6050Pi::SetI2CMasterModeEnabled(false);
	std::cout << "Setting slave 0 address to 0x68..." << std::endl;
	MPU6050Pi::SetSlaveAddress(0, 0x68);
	std::cout << "Resetting I2C Master control..." << std::endl;
	MPU6050Pi::ResetI2CMaster();
    std::this_thread::sleep_for (std::chrono::milliseconds(20));
	std::cout << "Setting clock source to Z Gyro..." << std::endl;
	MPU6050Pi::SetClockSource(CLOCK_PLL_ZGYRO);

	std::cout << "Setting DMP and FIFO_OFLOW interrupts enabled..." << std::endl;
	MPU6050Pi::SetIntEnabled(1<<FIFO_OFLOW_INT_BIT | 1<<DMP_INT_BIT);

	std::cout << "Setting sample rate to 200Hz..." << std::endl;
	MPU6050Pi::SetSampleRateDivider(4); // 1khz / (1 + 4) = 200 Hz


	std::cout << "Setting external frame sync to TEMP_OUT_L[0]..." << std::endl;
	MPU6050Pi::SetExternalFrameSync(EXT_SYNC_TEMP_OUT_L);

	std::cout << "Setting DLPF bandwidth to 42Hz..." << std::endl;
	MPU6050Pi::SetDLPFMode(DLPF_BW_42);

	std::cout << "Setting gyro sensitivity to +/- 2000 deg/sec..." << std::endl;
	MPU6050Pi::SetFullScaleGyroRange(FS_SEL_2000);

	// Load DMP code into memory banks
	std::cout << "Writing DMP code to MPU memory banks (" << DMP_CODE_SIZE << " bytes)... ";
    // if (!MPU6050Pi::WriteProgMemoryBlock(dmp_memory_, DMP_CODE_SIZE))
    //     return 1;
	std::cout << "Writing DMP code successful." << std::endl;

    // Set the FIFO rate divisor
    unsigned char dmpUpdate[] = {0x00, DMP_FIFO_RATE_DIVISOR};
    // Lets write the dmpUpdate data to the Firmware image, we have 2 bytes to write in bank 0x02 with the Offset 0x16
	MPU6050Pi::WriteMemoryBlock(dmpUpdate, 0x02, 0x02, 0x16);

    //Write start address MSB and LSB into register
	MPU6050Pi::SetDMPConfig1(0x03); //MSB
	MPU6050Pi::SetDMPConfig2(0x00); //LSB

	std::cout << "Clearing OTP Bank flag..." << std::endl;
	MPU6050Pi::SetOTPBankValid(false);

    // Setting motion detection threshold and duration
	std::cout << "Setting motion detection     : threshold =   2, duration = 80..." << std::endl;
	MPU6050Pi::SetMotionDetectionThreshold(2);
	MPU6050Pi::SetMotionDetectionDuration(80);

	// Setting Zero motion detection threshold and duration
	std::cout << "Setting zero-motion detection: threshold = 156, duration =  0..." << std::endl;
	MPU6050Pi::SetZeroMotionDetectionThreshold(156);
	MPU6050Pi::SetZeroMotionDetectionDuration(0);

	std::cout << "Enabling FIFO..." << std::endl;
	MPU6050Pi::SetFIFOEnabled(true);

	std::cout << "Resetting and disabling DMP..." << std::endl;
	MPU6050Pi::ResetDMP();
	MPU6050Pi::SetDMPEnabled(false);

	std::cout << "Setting up internal 42-byte (default) DMP packet buffer..." << std::endl;
	dmp_packet_size_ = 42;

	std::cout << "Resetting FIFO and clearing INT status one last time..." << std::endl;
	MPU6050Pi::ResetFIFO();
	MPU6050Pi::GetIntStatus();

    return 0;
}
