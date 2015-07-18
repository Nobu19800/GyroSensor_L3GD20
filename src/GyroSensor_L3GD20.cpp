// -*- C++ -*-
/*!
 * @file  GyroSensor_L3GD20.cpp
 * @brief GyroSensor_L3GD20
 * @date $Date$
 *
 * LGPL
 *
 * $Id$
 */

#include "GyroSensor_L3GD20.h"

// Module specification
// <rtc-template block="module_spec">
static const char* gyrosensor_l3gd20_spec[] =
  {
    "implementation_id", "GyroSensor_L3GD20",
    "type_name",         "GyroSensor_L3GD20",
    "description",       "GyroSensor_L3GD20",
    "version",           "1.0.0",
    "vendor",            "Miyamoto Nobuhiko",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.I2C_channel", "1",
    "conf.default.addr", "106",
    // Widget
    "conf.__widget__.I2C_channel", "radio",
    "conf.__widget__.addr", "text",
    // Constraints
    "conf.__constraints__.I2C_channel", "(1,6)",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
GyroSensor_L3GD20::GyroSensor_L3GD20(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_gyroOut("gyro", m_gyro)

    // </rtc-template>
{
	gyroSensor = NULL;
	_i2c = NULL;
}

/*!
 * @brief destructor
 */
GyroSensor_L3GD20::~GyroSensor_L3GD20()
{
}



RTC::ReturnCode_t GyroSensor_L3GD20::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("gyro", m_gyroOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("I2C_channel", m_I2C_channel, "1");
  bindParameter("addr", m_addr, "106");
  // </rtc-template>
  _smf = new i2c_smf();
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onFinalize()
{
	if(gyroSensor)
	{
		delete gyroSensor;
	}
	if(_i2c)
	{
		_smf->sem_lock();
		delete _i2c;
		_smf->sem_unlock();
	}

	delete _smf;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onActivated(RTC::UniqueId ec_id)
{
	mraa_result_t response;
	if(_i2c == NULL)
	{
		_smf->sem_lock();
		_i2c = new mraa::I2c(m_I2C_channel);
		_smf->sem_unlock();
	}
	if(gyroSensor == NULL)
	{
		gyroSensor = new L3GD20(_i2c, _smf, m_addr);
		if(response != MRAA_SUCCESS)
		{
			return RTC::RTC_ERROR;
		}
	}

	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onExecute(RTC::UniqueId ec_id)
{
	double avx = 0;
	double avy = 0;
	double avz = 0;

	if(gyroSensor)
	{
		
		gyroSensor->getGyro(avx,avy,avz);
		m_gyro.data.avx = avx;
		m_gyro.data.avy = avy;
		m_gyro.data.avz = avz;
		
		m_gyroOut.write();
	}

  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t GyroSensor_L3GD20::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}



extern "C"
{
 
  void GyroSensor_L3GD20Init(RTC::Manager* manager)
  {
    coil::Properties profile(gyrosensor_l3gd20_spec);
    manager->registerFactory(profile,
                             RTC::Create<GyroSensor_L3GD20>,
                             RTC::Delete<GyroSensor_L3GD20>);
  }
  
};


