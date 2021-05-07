#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <CommonType.h>


typedef enum {
	AFS_SEL_2g,
	AFS_SEL_4g,
	AFS_SEL_8g,
	AFS_SEL_16g
}Mpu9250_ImuScale_t; // [LSB/g]


typedef enum {
	FS_SEL_250dps,
	FS_SEL_500dps,
	FS_SEL_1000dps,
	FS_SEL_2000dps
}Mpu9250_GyroScale_t; // [Deg/g]

// MPU9250
#define READ_FLAG 0x80
// Configuration Address
#define MPUREG_SMPLRT_DIV		0x19
#define MPUREG_CONFIG			0x1A
#define MPUREG_GYRO_CONFIG		0x1B
#define MPUREG_ACC_CONFIG		0x1C
#define MPUREG_ACC_CONFIG_2 		0x1D
#define MPUREG_USER_CTRL		0x6A
#define MPUREG_PWR_MGMT_1		0x6B
#define MPUREG_PWR_MGMT_2		0x6C
#define MPUREG_WHO_AM_I			0x75
#define MPUREG_I2C_MST_CTRL		0x24

// Default Value
#define MPU_ID					0x71
#define PWR_MGMT_RST			0x80
#define PWR_MGMT_CLKSEL			0x1 << 1
#define USER_CTRL_I2C_MST_EN	0x1 << 5
#define USER_CTRL_I2C_IF_DIS	0x1 << 4

#define I2C_MST_CTRL_400KHZ		0x0D

// Data Address
// Acceleration
#define MPUREG_ACC_X_H			0x3B
#define MPUREG_ACC_X_L			0x3C
#define MPUREG_ACC_Y_H			0x3D
#define MPUREG_ACC_Y_L			0x3E
#define MPUREG_ACC_Z_H			0x3F
#define MPUREG_ACC_Z_L			0x40

// Temp
#define MPUREG_TEMP_H			0x41
#define MPUREG_TEMP_L			0x42

// Gyro
#define MPUREG_GYRO_X_H			0x43 
#define MPUREG_GYRO_X_L			0x44
#define MPUREG_GYRO_Y_H			0x45
#define MPUREG_GYRO_Y_L			0x46
#define MPUREG_GYRO_Z_H			0x47
#define MPUREG_GYRO_Z_L			0x48

// Slave Configuration
#define MPUREG_I2C_SLV0_ADDR		0x25
#define MPUREG_I2C_SLV0_REG			0x26
#define MPUREG_I2C_SLV0_CTRL		0x27
#define MPUREG_I2C_SLV0_CTRL_EN		0x1 << 7	
//#define MPUREG_I2C_SLV0_CTRL_LEN(x)	x	

#define MPUREG_I2C_SLV0_DO         0x63
#define MPUREG_I2C_SLV1_ADDR       0x28
#define MPUREG_I2C_SLV1_REG        0x29
#define MPUREG_I2C_SLV1_CTRL       0x2A
#define MPUREG_I2C_SLV1_DO         0x64
#define MPUREG_I2C_SLV2_ADDR       0x2B
#define MPUREG_I2C_SLV2_REG        0x2C
#define MPUREG_I2C_SLV2_CTRL       0x2D
#define MPUREG_I2C_SLV2_DO         0x65
#define MPUREG_I2C_SLV3_ADDR       0x2E
#define MPUREG_I2C_SLV3_REG        0x2F
#define MPUREG_I2C_SLV3_CTRL       0x30
#define MPUREG_I2C_SLV3_DO         0x66
#define MPUREG_I2C_SLV4_ADDR       0x31
#define MPUREG_I2C_SLV4_REG        0x32
#define MPUREG_I2C_SLV4_DO         0x33
#define MPUREG_I2C_SLV4_CTRL       0x34
#define MPUREG_I2C_SLV4_DI         0x35
#define MPUREG_I2C_MST_STATUS      0x36

#define MPUREG_EXT_SENS_DATA_00    0x49
#define MPUREG_EXT_SENS_DATA_01    0x4A
#define MPUREG_EXT_SENS_DATA_02    0x4B
#define MPUREG_EXT_SENS_DATA_03    0x4C
#define MPUREG_EXT_SENS_DATA_04    0x4D
#define MPUREG_EXT_SENS_DATA_05    0x4E
#define MPUREG_EXT_SENS_DATA_06    0x4F
#define MPUREG_EXT_SENS_DATA_07    0x50
#define MPUREG_EXT_SENS_DATA_08    0x51
#define MPUREG_EXT_SENS_DATA_09    0x52
#define MPUREG_EXT_SENS_DATA_10    0x53
#define MPUREG_EXT_SENS_DATA_11    0x54
#define MPUREG_EXT_SENS_DATA_12    0x55
#define MPUREG_EXT_SENS_DATA_13    0x56
#define MPUREG_EXT_SENS_DATA_14    0x57
#define MPUREG_EXT_SENS_DATA_15    0x58
#define MPUREG_EXT_SENS_DATA_16    0x59
#define MPUREG_EXT_SENS_DATA_17    0x5A
#define MPUREG_EXT_SENS_DATA_18    0x5B
#define MPUREG_EXT_SENS_DATA_19    0x5C
#define MPUREG_EXT_SENS_DATA_20    0x5D
#define MPUREG_EXT_SENS_DATA_21    0x5E
#define MPUREG_EXT_SENS_DATA_22    0x5F
#define MPUREG_EXT_SENS_DATA_23    0x60

// AK8963
#define AK8963_I2C_ADDR             0x0C 
#define AK8963_Device_ID            0x48
// Read-only Reg
#define AK8963_WIA			0x00
#define AK8963_INFO			0x01
#define AK8963_ST1			0x02
#define AK8963_HXL			0x03
#define AK8963_HXH			0x04
#define AK8963_HYL			0x05
#define AK8963_HYH			0x06
#define AK8963_HZL			0x07
#define AK8963_HZH			0x08
#define AK8963_ST2			0x09
// Write/Read Reg
#define AK8963_CNTL1		0x0A
#define AK8963_CNTL1_MODE1  0x01 << 2
#define AK8963_CNTL1_16BIT  0x01 << 4
#define AK8963_CNTL2		0x0B
#define AK8963_CNTL2_RST	0x01
#define AK8963_ASTC			0x0C
#define AK8963_TS1			0x0D
#define AK8963_TS2			0x0E
#define AK8963_I2CDIS		0x0F
// Read-only Reg ( ROM )
#define AK8963_ASAX			0x10
#define AK8963_ASAY			0x11
#define AK8963_ASAZ			0x12

enum SPI_CLK_t {
	SPI_LS = 1000000,
	SPI_HS = 20000000
};

enum MPU_INTERFACE {
	USE_SPI = 0,
	USE_I2C = 1
};

enum MPU_STATUS {
	MPU_FAIL = 0, 
	MPU_OK
};

class MPU9250 {
public:
	MPU9250();

	void setup(SPIClass* spix, uint8_t cs);
	void setup(TwoWire* bus, uint8_t i2cAddr);
	MPU_STATUS begin();

	~MPU9250();

	void writeRegister(const uint8_t regAddr, uint8_t value, bool checkFlag = false);
	int readRegister(const uint8_t regAddr, const uint8_t nBytes, uint8_t* rxBuf);

	void writeAK8963Reg(const uint8_t subReg, uint8_t value, bool checkFlag = false);
	void readAK8963Reg(const uint8_t regAddr, const uint8_t nBytes, uint8_t* rxBuf, bool checkFlag = false);

	void setSensitivity(Mpu9250_ImuScale_t imuScale, Mpu9250_GyroScale_t gyroScale);

	void sample();
	void printData();
private:
	MPU_INTERFACE _interface;
	SPIClass* _spiPort;
	SPISettings _spiLS = SPISettings(1000000, MSBFIRST, SPI_MODE3);
	SPISettings _spiHS = SPISettings(20000000, MSBFIRST, SPI_MODE3);

	uint8_t _address;	
	TwoWire* _i2cPort;

	uint8_t _CSPort;
	SPI_CLK_t _spiFreq;

	void requestAcc();
	void requestGyro();
	void requestMag();

	double _imuScale = 1;
	Acceleration_t _accel;
	Acceleration_t _imuBias;

	double _gyroScale = 1;
	AttitudeRate_t _attRate;
	AttitudeRate_t _gyroBias;

	Magnetometer_t _mag;

	float temparature;
};



