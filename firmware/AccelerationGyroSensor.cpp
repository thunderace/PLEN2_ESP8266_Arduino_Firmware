/*
	Copyright (c) 2015,
	- Kazuyuki TAKASE - https://github.com/junbowu
	- PLEN Project Company Inc. - https://plen.jp

	This software is released under the MIT License.
	(See also : http://opensource.org/licenses/mit-license.php)
*/
#include <Arduino.h>
#include "firmware.h"
#include "Wire.h"
#include "Pin.h"
#include "System.h"
#include "AccelerationGyroSensor.h"

#if DEBUG
	#include "Profiler.h"
#endif


namespace
{
	template<typename T>
	void endian_cast(T& value)
	{
		char  temp;
		char* filler = reinterpret_cast<char*>(&value);

		for (int index = 0; index < (sizeof(T) / 2); index++)
		{
			temp = filler[sizeof(T) - 1 - index];
			filler[sizeof(T) - 1 - index] = filler[index];
			filler[index] = temp;
		}
	}

	template<>
	void endian_cast(int& value)
	{
		value = ((value & 0x00FF) << 8) | ((value >> 8) & 0x00FF);
	}
}

void PLEN2::AccelerationGyroSensor::setup() {
	delay(150);
	I2C_Write(MPU6050_REGISTER_SMPLRT_DIV, 0x07);
	I2C_Write(MPU6050_REGISTER_PWR_MGMT_1, 0x01);
	I2C_Write(MPU6050_REGISTER_PWR_MGMT_2, 0x00);
	I2C_Write(MPU6050_REGISTER_CONFIG, 0x00);
	I2C_Write(MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
	I2C_Write(MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
	I2C_Write(MPU6050_REGISTER_FIFO_EN, 0x00);
	I2C_Write(MPU6050_REGISTER_INT_ENABLE, 0x01);
	I2C_Write(MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
	I2C_Write(MPU6050_REGISTER_USER_CTRL, 0x00);

}

void PLEN2::AccelerationGyroSensor::I2C_Write(uint8_t regAddress, uint8_t data) {
	Wire.beginTransmission(MPU6050SlaveAddress);
	Wire.write(regAddress);
	Wire.write(data);
	Wire.endTransmission();
}

void PLEN2::AccelerationGyroSensor::sampling()
{
#if MPU_6050
	Wire.beginTransmission(MPU6050SlaveAddress);
	Wire.write(MPU6050_REGISTER_ACCEL_XOUT_H);
	Wire.endTransmission();
	Wire.requestFrom(MPU6050SlaveAddress, (uint8_t)14);
	m_values[ACC_X] = (((int16_t)Wire.read() << 8) | Wire.read());
	m_values[ACC_Y] = (((int16_t)Wire.read() << 8) | Wire.read());
	m_values[ACC_Z] = (((int16_t)Wire.read() << 8) | Wire.read());
	//Temperature = (((int16_t)Wire.read() << 8) | Wire.read());
	m_values[GYRO_ROLL] = (((int16_t)Wire.read() << 8) | Wire.read());
	m_values[GYRO_PITCH] = (((int16_t)Wire.read() << 8) | Wire.read());
	m_values[GYRO_YAW] = (((int16_t)Wire.read() << 8) | Wire.read());
#else
	return;
#endif
}

// raw values
const int& PLEN2::AccelerationGyroSensor::getAccX()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getAccX()"));
	#endif

	return m_values[ACC_X];
}

const int& PLEN2::AccelerationGyroSensor::getAccY()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getAccY()"));
	#endif

	return m_values[ACC_Y];
}

const int& PLEN2::AccelerationGyroSensor::getAccZ()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getAccZ()"));
	#endif

	return m_values[ACC_Z];
}

const int& PLEN2::AccelerationGyroSensor::getGyroRoll()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getGyroRoll()"));
	#endif

	return m_values[GYRO_ROLL];
}

const int& PLEN2::AccelerationGyroSensor::getGyroPitch()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getGyroPitch()"));
	#endif

	return m_values[GYRO_PITCH];
}

const int& PLEN2::AccelerationGyroSensor::getGyroYaw()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::getGyroYaw()"));
	#endif

	return m_values[GYRO_YAW];
}

#if 0
// values wityh sensitivity
const double& PLEN2::AccelerationGyroSensor::getAccX()
{
	return (double)m_values[ACC_X]/AccelScaleFactor;
}

const double& PLEN2::AccelerationGyroSensor::getAccY()
{
	return (double)m_values[ACC_Y] / AccelScaleFactor;
}

const double& PLEN2::AccelerationGyroSensor::getAccZ()
{
	return (double)m_values[ACC_Z] / AccelScaleFactor;
}

const double& PLEN2::AccelerationGyroSensor::getGyroRoll()
{
	return (double)m_values[GYRO_ROLL] / AccelScaleFactor;
}

const double& PLEN2::AccelerationGyroSensor::getGyroPitch()
{
	return (double)m_values[GYRO_PITCH] / AccelScaleFactor;
}

const double& PLEN2::AccelerationGyroSensor::getGyroYaw()
{
	return (double)m_values[GYRO_YAW] / AccelScaleFactor;
}
#endif


void PLEN2::AccelerationGyroSensor::dump()
{
	#if DEBUG
		volatile Utility::Profiler p(F("AccelerationGyroSensor::dump()"));
	#endif

	sampling();

	System::outputSerial().println(F("{"));

	System::outputSerial().print(F("\t\"Acc X\": "));
	System::outputSerial().print(getAccX());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Acc Y\": "));
	System::outputSerial().print(getAccY());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Acc Z\": "));
	System::outputSerial().print(getAccZ());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Gyro Roll\": "));
	System::outputSerial().print(getGyroRoll());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Gyro Pitch\": "));
	System::outputSerial().print(getGyroPitch());
	System::outputSerial().println(F(","));

	System::outputSerial().print(F("\t\"Gyro Yaw\": "));
	System::outputSerial().println(getGyroYaw());

	System::outputSerial().println(F("}"));
}
