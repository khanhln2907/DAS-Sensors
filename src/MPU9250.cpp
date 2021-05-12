#include "MPU9250.h"
#include "ArduinoLog.h"

#define EN_CS digitalWrite(_CSPort,LOW)
#define DIS_CS digitalWrite(_CSPort,HIGH)



MPU9250::MPU9250()
{
	;
}

void MPU9250::setup(SPIClass* spix, uint8_t cs){
	_interface = USE_SPI;
	_spiPort = spix;
	_CSPort = cs;
	pinMode(_CSPort, OUTPUT);
	_spiPort->begin();
}


void MPU9250::setup(TwoWire* bus, uint8_t i2cAddr){
	_interface = USE_I2C;
	_i2cPort = bus;
	_address = i2cAddr;
	_i2cPort->begin();
}

MPU_CONFIG_STATUS MPU9250::begin()
{
	// Debugging
	bool isDoubleCheck = false;

	// Reset power of MPU to stabilize the SPI protocols
	writeRegister(MPUREG_PWR_MGMT_1, PWR_MGMT_RST);
	delay(100);

	uint8_t checkIDMPU;
	readRegister(MPUREG_WHO_AM_I, 1, &checkIDMPU);
	if (checkIDMPU != MPU_ID) 
		return MPU_CONFIG_STATUS::MPU_ID_FAIL;

	// Select source clock
	writeRegister(MPUREG_PWR_MGMT_1, PWR_MGMT_CLKSEL);

	// Enable I2C and set the speed to 400kHz
	uint8_t UserControlConfig = USER_CTRL_I2C_MST_EN | USER_CTRL_I2C_IF_DIS; // Bring this later outside for setting up
	if(writeRegister(MPUREG_USER_CTRL, UserControlConfig, isDoubleCheck) != MPU_RW_STATUS::WRITE_SUCCESS)
		return MPU_USER_CTRL_FAIL;		// Enable I2C for external sensors
	
	if(writeRegister(MPUREG_I2C_MST_CTRL, I2C_MST_CTRL_400KHZ, isDoubleCheck) != MPU_RW_STATUS::WRITE_SUCCESS)
		return MPU_I2C_MST_CTR_FAIL;  	// I2C at 400Khz

	// Communication with AK8963: reset, configure mode
	if(writeRegister(MPUREG_I2C_SLV0_ADDR, AK8963_I2C_ADDR, isDoubleCheck) != MPU_RW_STATUS::WRITE_SUCCESS)
		return  MPU_SLV0_ADDR_FAIL;		// Set address of slave (AK8963 I2C: 0x0C -> Slave 0) in write mode
	
	uint8_t checkIDAK8963;
	if(writeAK8963Reg(AK8963_CNTL2, AK8963_CNTL2_RST, isDoubleCheck) != MPU_AK8963_STATUS::WRITE_SUCCESS) 
		return MPU_AK8963_CTRL_FAIL;
	
	// Ask ID of AK8963
	readAK8963Reg(AK8963_WIA, 1, &checkIDAK8963); // Double check is not available
	if(checkIDAK8963 != AK8963_Device_ID) 
		return MPU_AK8963_ID_FAIL;

	if(writeAK8963Reg(AK8963_CNTL1, AK8963_CNTL1_MODE1 | AK8963_CNTL1_16BIT, isDoubleCheck) != MPU_AK8963_STATUS::WRITE_SUCCESS) 
		return MPU_AK8963_FAIL; 
	
	// Enable data streaming of AK8963
	// Enable reading 1 Byte from of the slave at own sample rate.
	if(writeRegister(MPUREG_I2C_SLV0_CTRL, MPUREG_I2C_SLV0_CTRL_EN | 1, isDoubleCheck) != MPU_RW_STATUS::WRITE_SUCCESS) 
		return MPU_SLV0_CTRL_FAIL; 
	
																						
	// Setup bandwidth and DSP
	if(configureDLP() == false)
		return MPU_FAIL;



	// Ready to go
	//Log.notice("MPU9250 Startup sucessfully! ");
	return MPU_OK;
}

MPU9250::~MPU9250()
{
	_spiPort->end();
}

MPU_RW_STATUS MPU9250::writeRegister(const uint8_t regAddr, uint8_t data, bool checkFlag)
{
	if(_interface == USE_SPI){
		// Config SPI Here
		_spiPort->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
		// Start Data Transmission
		EN_CS;
		_spiPort->transfer(regAddr);
		_spiPort->transfer(data);
		DIS_CS;
		_spiPort->endTransaction();
	}
	else {
		_i2cPort->beginTransmission(_address); // open the device
		_i2cPort->write(regAddr); // write the register address
		_i2cPort->write(data); // write the data
		_i2cPort->endTransmission();
	}
	if (checkFlag) {
		// Checking function
		delayMicroseconds(10);
		uint8_t readback;
		readRegister(regAddr, 1, &readback);
		//Log.notice("Address: %X | Written: %X | Readback: %X \n", regAddr, data, tmp);
		//Log.notice("R: %X W: %X Rx: %X \n", regAddr, data, readback);
		if(readback == data)
			return MPU_RW_STATUS::WRITE_SUCCESS;
		else
			return MPU_RW_STATUS::WRITE_FAIL;
	}
	// If no double check is required, just sendback success
	return MPU_RW_STATUS::WRITE_SUCCESS;
}

MPU_RW_STATUS MPU9250::readRegister(const uint8_t regAddr, const uint8_t nBytes, uint8_t* rxBuf)
{
	if(_interface == USE_SPI){
		// Config SPI Here
		//_spiPort->beginTransaction(_spiLS);
		_spiPort->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));

		// Start Data Transmission
		EN_CS;
		_spiPort->transfer(0x80 | regAddr);

		for (uint8_t i = 0; i < nBytes; i++) {
			rxBuf[i] = _spiPort->transfer(0x00);
		}
		DIS_CS;

		_spiPort->endTransaction();
		return MPU_RW_STATUS::READ_SUCCESS;
	}
	else {
		_i2cPort->beginTransmission(_address); // open the device
		_i2cPort->write(regAddr); // specify the starting register address
		_i2cPort->endTransmission(false);
		auto nBytesGet = _i2cPort->requestFrom(_address, nBytes); // specify the number of bytes to receive
		if (nBytesGet == nBytes) {
			for (uint8_t i = 0; i < nBytes; i++) {
				rxBuf[i] = _i2cPort->read();
			}
			return MPU_RW_STATUS::READ_SUCCESS;
		}
		else {
			return MPU_RW_STATUS::READ_FAIL;
		}
		_i2cPort->endTransmission();
	}
}

MPU_AK8963_STATUS MPU9250::writeAK8963Reg(const uint8_t subReg, uint8_t value, bool checkFlag)
{
	// //Set the I2C slave addres of AK8963 and set to write
	// // Not allow to readback the value
	// writeRegister(MPUREG_I2C_SLV0_ADDR, AK8963_I2C_ADDR, checkFlag = false);
	// writeRegister(MPUREG_I2C_SLV0_REG, subReg, checkFlag = false); //I2C slave 0 register address from where to begin data transfer
	// writeRegister(MPUREG_I2C_SLV0_DO, value, checkFlag = false); //Read 1 byte from the magnetometer
	// return MPU_AK8963_STATUS::READ_SUCCESS;
	//Set the I2C slave addres of AK8963 and set to write
	// Not allow to readback the value
	auto st1 = writeRegister(MPUREG_I2C_SLV0_ADDR, AK8963_I2C_ADDR, checkFlag);
	auto st2 = writeRegister(MPUREG_I2C_SLV0_REG, subReg, checkFlag); //I2C slave 0 register address from where to begin data transfer
	auto st3 = writeRegister(MPUREG_I2C_SLV0_DO, value, checkFlag); //Read 1 byte from the magnetometer
	if((st1 != MPU_RW_STATUS::WRITE_SUCCESS) | (st2 != MPU_RW_STATUS::WRITE_SUCCESS) | (st3 != MPU_RW_STATUS::WRITE_SUCCESS))
		return MPU_AK8963_STATUS::WRITE_FAIL;
	else
		return MPU_AK8963_STATUS::WRITE_SUCCESS;
}

// Attention: blocking
MPU_AK8963_STATUS MPU9250::readAK8963Reg(const uint8_t regAddr, const uint8_t nBytes, uint8_t* rxBuf, bool checkFlag)
{
	auto status1 = writeRegister(MPUREG_I2C_SLV0_ADDR, READ_FLAG | AK8963_I2C_ADDR, checkFlag); // Set read slave 0
	auto status2 = writeRegister(MPUREG_I2C_SLV0_REG, regAddr, checkFlag);
	auto status3 = writeRegister(MPUREG_I2C_SLV0_CTRL, MPUREG_I2C_SLV0_CTRL_EN | (nBytes), checkFlag); //Read 1 byte

	if(status1 != MPU_RW_STATUS::WRITE_SUCCESS | status2 != MPU_RW_STATUS::WRITE_SUCCESS | status3 != MPU_RW_STATUS::WRITE_SUCCESS)
		return MPU_AK8963_STATUS::READ_FAIL;
	else{
		// wait until new data is availble
		delay(50);
		// Read here
		if(readRegister(MPUREG_EXT_SENS_DATA_00, nBytes, rxBuf)!= MPU_RW_STATUS::READ_SUCCESS)
			return MPU_AK8963_STATUS::READ_FAIL;
	}

	return MPU_AK8963_STATUS::READ_SUCCESS;
}

void MPU9250::setSensitivity(Mpu9250_ImuScale_t imuScale, Mpu9250_GyroScale_t gyroScale)
{
	_imuScale = imuScale;
	_gyroScale = gyroScale;

	// Update to MPU
}


void MPU9250::requestAcc()
{
	uint8_t response[6];
	readRegister(MPUREG_ACC_X_H, 6, response);
	_accel.x = (double)(((int16_t)response[0] << 8) | response[1])/ _imuScale - _imuBias.x;
	_accel.y = (double)(((int16_t)response[2] << 8) | response[3])/ _imuScale - _imuBias.y;
	_accel.z = (double)(((int16_t)response[4] << 8) | response[5])/ _imuScale - _imuBias.z;
}

void MPU9250::requestGyro()
{
	uint8_t response[6];
	readRegister(MPUREG_GYRO_X_H, 6, response);
	_attRate.p = (double)(((int16_t)response[0] << 8) | response[1]) / _gyroScale - _gyroBias.p;
	_attRate.q = (double)(((int16_t)response[2] << 8) | response[3]) / _gyroScale - _gyroBias.q;
	_attRate.r = (double)(((int16_t)response[4] << 8) | response[5]) / _gyroScale - _gyroBias.r;
}

void MPU9250::requestMag()
{
	uint8_t response[7];
	float data;
	int i;

	readAK8963Reg(AK8963_HXL, 7, response); // Blocking inside, must be debugged and evaluated. // Request AK89
	readRegister(MPUREG_EXT_SENS_DATA_00, 7, response);

	//writeRegister(MPUREG_I2C_SLV0_ADDR, AK8963_I2C_ADDR | READ_FLAG);  // Set the I2C slave addres of AK8963 and set for read.
	//writeRegister(MPUREG_I2C_SLV0_REG, AK8963_HXL);                 // I2C slave 0 register address from where to begin data transfer
	//writeRegister(MPUREG_I2C_SLV0_CTRL, 0x87);                      // Read 6 bytes from the magnetometer
	//delayMicroseconds(50); 
	//readRegister(MPUREG_EXT_SENS_DATA_00, 7, response);
	// must start your read from AK8963A register 0x03 and read seven bytes so that upon read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.
	_mag.x = (double) (((int16_t)response[1] << 8) | response[0]); // Must be compensated as well fuck !
	_mag.y = (double)(((int16_t)response[3] << 8) | response[2]);
	_mag.z = (double)(((int16_t)response[5] << 8) | response[4]);

	// mag_data[i] = data * Magnetometer_ASA[i];
}

void MPU9250::sample()
{
	uint8_t response[21];
	readRegister(MPUREG_ACC_X_H, 21, response);
	 /*Log.notice("Data buffer: ");
	 for (int i = 0; i < 20; i++) {
	 	Log.notice(" %X ", response[i]);
	 }
	 Log.notice("\n");*/
	_imuScale = 16384;
	_gyroScale = 8192 / 125;
	 _accel.x = (double)(((int16_t)response[0] << 8) | response[1]) / _imuScale - _imuBias.x;
	 _accel.y = (double)(((int16_t)response[2] << 8) | response[3]) / _imuScale - _imuBias.y;
	 _accel.z = (double)(((int16_t)response[4] << 8) | response[5]) / _imuScale - _imuBias.z;
	 temparature = (float)(((int16_t)response[6] << 8) | response[7]);
	 _attRate.p = (double)(((int16_t)response[8] << 8) | response[9]) / _gyroScale - _gyroBias.p;
	 _attRate.q = (double)(((int16_t)response[10] << 8) | response[11]) / _gyroScale - _gyroBias.q;
	 _attRate.r = (double)(((int16_t)response[12] << 8) | response[13]) / _gyroScale - _gyroBias.r;
	 _mag.x = (double)(((int16_t)response[15] << 8) | response[14]); // Must be compensated as well fuck !
	 _mag.y = (double)(((int16_t)response[17] << 8) | response[16]);
	 _mag.z = (double)(((int16_t)response[19] << 8) | response[18]);
}


void MPU9250::printData()
{
	char str[50];
	sprintf(str, "Time: %d T: %.3f \n \n", (unsigned long) micros(), temparature);
	Log.notice(str);
	sprintf(str, "A: x: %.3f  y: %.3f  z: %.3f \n", _accel.x, _accel.y, _accel.z);
	Log.notice(str);
	sprintf(str, "G: p: %.3f  q: %.3f  r: %.3f \n", _attRate.p, _attRate.q, _attRate.r);
	Log.notice(str);
	sprintf(str, "M: x: %.3f  y: %.3f  z: %.3f \n", _mag.x, _mag.y, _mag.z);
	Log.notice(str);
	
}

bool MPU9250::configureDLP()
{
	byte enDLP = 0b11; // Default Gyro scale, turn off DLP
	if(writeRegister(MPUREG_GYRO_CONFIG, enDLP, true) != MPU_RW_STATUS::WRITE_SUCCESS)
		return false;
	
	byte deAccFilter = 0b1 << 3; // Default Gyro scale, turn off DLP
	if(writeRegister(MPUREG_ACC_CONFIG_2, deAccFilter, true) != MPU_RW_STATUS::WRITE_SUCCESS)
		return false;




}