/**
 * @file EnvironmentMonitor.cpp
 * @copyright 2015 Bernd Wachter
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2015
 */

#include <Arduino.h>

#if ENVIRONMENTMONITOR_SENSOR_BME280 > 0
 #if ENVIRONMENTMONITOR_BME_LIBRARY == L_Glenn || ENVIRONMENTMONITOR_BME_LIBRARY == L_SparkFun
#include <Wire.h>
 #endif
#endif

#include "EnvironmentMonitor.h"

EnvironmentMonitor::EnvironmentMonitor(){
}

bool EnvironmentMonitor::begin(){
  // TODO: check configuration structure with sensor data
  startSensors();
  return true;
}

#if ENVIRONMENTMONITOR_LPM > 0
void EnvironmentMonitor::sleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  delay(100);
  sleep_mode();
  sleep_disable();
}
#endif

measurement* EnvironmentMonitor::data(byte *ptr){
  *ptr = m_data_p;
  return m_data;
}

bool EnvironmentMonitor::pollSensors(){
  if (m_pollCnt >= ENVIRONMENTMONITOR_HISTORY_LENGTH && !m_ready)
    m_ready = true;

#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
  if (sensors.bmp085){
    m_data[m_data_p].bmp085.temperature = bmp.readTemperature();
    // store the pressure in easier to use hPA
    m_data[m_data_p].bmp085.pressure = bmp.readPressure()/100.0;
  }
#endif

#if ENVIRONMENTMONITOR_SENSOR_BME280 > 0
  if (sensors.bme280){
 #if ENVIRONMENTMONITOR_BME_LIBRARY == L_Adafruit
    m_data[m_data_p].bme280.temperature = bme.readTemperature();
      // store the pressure in easier to use hPA
    m_data[m_data_p].bme280.pressure = bme.readPressure()/100.0;
    m_data[m_data_p].bme280.humidity = bme.readHumidity();
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Bosch
    bool ok = bme.measure();
    if (ok){
      m_data[m_data_p].bme280.temperature = bme.getTemperature()/100.0;
      // store the pressure in easier to use hPA
      m_data[m_data_p].bme280.pressure = bme.getPressure()/100.0;
      m_data[m_data_p].bme280.humidity = bme.getHumidity()/1024.0;
    }
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Glenn
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);

    bme.read(m_data[m_data_p].bme280.pressure,
             m_data[m_data_p].bme280.temperature,
             m_data[m_data_p].bme280.humidity,
             tempUnit,
             presUnit);
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_SparkFun
    m_data[m_data_p].bme280.temperature = bme.readTempC();
      // store the pressure in easier to use hPA
    m_data[m_data_p].bme280.pressure = bme.readFloatPressure()/100.0;
    m_data[m_data_p].bme280.humidity = bme.readFloatHumidity();
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Forced
    bme.takeForcedMeasurement();
    m_data[m_data_p].bme280.temperature = bme.getTemperatureCelcius();
    m_data[m_data_p].bme280.pressure = bme.getPressure();
    m_data[m_data_p].bme280.humidity = bme.getRelativeHumidity();
 #endif
  }
#endif

#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
  if (sensors.mcp9808){
#if ENVIRONMENTMONITOR_LPM > 0
    mcp9808.shutdown_wake(0);
#endif
    m_data[m_data_p].mcp9808.temperature = mcp9808.readTempC();
#if ENVIRONMENTMONITOR_LPM > 0
    mcp9808.shutdown_wake(1);
#endif
  }
#endif

#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
  if (sensors.dht22){
    m_data[m_data_p].dht22.temperature = dht22.readTemperature();
    m_data[m_data_p].dht22.humidity = dht22.readHumidity();
  }
#endif

#if ENVIRONMENTMONITOR_SENSOR_RAIN > 0
  // we can't autodetect presence of the rain sensor, so just read values if
  // it is compiled in.
  if (digitalRead(ENVIRONMENTMONITOR_RAIN_INPUT) == LOW){
    m_data[m_data_p].rain.rain = analogRead(ENVIRONMENTMONITOR_RAIN_INPUT);
  } else {
    m_data[m_data_p].rain.rain = 0;
  }
#endif

  if (m_data_p == ENVIRONMENTMONITOR_HISTORY_LENGTH - 1)
    m_data_p = 0;
  else
    m_data_p++;

  m_pollCnt++;
  return m_ready;
}

void EnvironmentMonitor::startSensors(){
#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
  if (bmp.begin())
    sensors.bmp085 = true;
#endif
#if ENVIRONMENTMONITOR_SENSOR_BME280 > 0
 #if ENVIRONMENTMONITOR_BME_LIBRARY == L_Adafruit
  if (bme.begin())
    sensors.bme280 = true;
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Bosch
  if (bme.beginI2C(0x77))
    sensors.bme280 = true;
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Glenn
  Wire.begin();
  if (bme.begin())
    sensors.bme280 = true;
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_SparkFun
  Wire.begin();
  if (bme.beginI2C())
    sensors.bme280 = true;
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Forced
  Wire.begin();
  bme.begin();
  // forced sensor doesn't check availability
  sensors.bme280 = true;
 #endif
#endif
#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
  if (mcp9808.begin())
    sensors.mcp9808 = true;
#endif
#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
  dht22.begin();
  sensors.dht22 = true;
#endif
#if ENVIRONMENTMONITOR_SENSOR_RAIN > 0
  // rain sensor
  pinMode(ENVIRONMENTMONITOR_RAIN_INPUT, INPUT);
  // rain sensor voltage
  pinMode(ENVIRONMENTMONITOR_RAIN_OUTPUT, OUTPUT);
#endif
}

bool EnvironmentMonitor::hasBMP085(){
#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
  return sensors.bmp085;
#else
  return false;
#endif
}

bool EnvironmentMonitor::hasBME280(){
#if ENVIRONMENTMONITOR_SENSOR_BME280 > 0
  return sensors.bme280;
#else
  return false;
#endif
}

bool EnvironmentMonitor::hasMCP9808(){
  #if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
  return sensors.mcp9808;
#else
  return false;
#endif
}

bool EnvironmentMonitor::hasDHT22(){
#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
  return sensors.dht22;
#else
  return false;
#endif
}
