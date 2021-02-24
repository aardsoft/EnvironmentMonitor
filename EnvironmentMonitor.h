/**
 * @file EnvironmentMonitor.h
 * @copyright 2015 Bernd Wachter
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2015
 */

#ifndef _ENVIRONMENTMONITOR_H
#define _ENVIRONMENTMONITOR_H

#define L_Adafruit 1
#define L_Bosch 2
#define L_SparkFun 3
#define L_Glenn 4
#define L_Forced 5


#ifndef ENVIRONMENTMONITOR_SENSOR_BMP085
#define ENVIRONMENTMONITOR_SENSOR_BMP085 0
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_BME280
#define ENVIRONMENTMONITOR_SENSOR_BME280 0
#endif

#ifndef ENVIRONMENTMONITOR_BME_LIBRARY
#define ENVIRONMENTMONITOR_BME_LIBRARY L_Adafruit
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_MCP9808
#define ENVIRONMENTMONITOR_SENSOR_MCP9808 0
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_DHT22
#define ENVIRONMENTMONITOR_SENSOR_DHT22 0
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_RAIN
#define ENVIRONMENTMONITOR_SENSOR_RAIN 0
#endif

#ifndef ENVIRONMENTMONITOR_RAIN_INPUT
#define ENVIRONMENTMONITOR_RAIN_INPUT 8
#endif

#ifndef ENVIRONMENTMONITOR_RAIN_OUTPUT
#define ENVIRONMENTMONITOR_RAIN_OUTPUT 3
#endif

// the purpose for this is to keep history on unreliable transmission media as
// well as being able to calculate averages
#ifndef ENVIRONMENTMONITOR_HISTORY_LENGTH
#define ENVIRONMENTMONITOR_HISTORY_LENGTH 1
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_IDENTIFIER
#define ENVIRONMENTMONITOR_SENSOR_IDENTIFIER 0
#endif

#include "EnvironmentMonitor_types.h"

#ifndef ENVIRONMENTMONITOR_LPM
#define ENVIRONMENTMONITOR_LPM 0
#endif

#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
#include <Adafruit_BMP085.h>
#endif

#if ENVIRONMENTMONITOR_SENSOR_BME280 > 0
 #if ENVIRONMENTMONITOR_BME_LIBRARY == L_Adafruit
#include <Adafruit_BME280.h>
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Bosch
#include <Bme280BoschWrapper.h>
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Glenn
#include <BME280I2C.h>
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_SparkFun
#include <SparkFunBME280.h>
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Forced
#include <forcedClimate.h>
 #endif
#endif

#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
#include <Adafruit_MCP9808.h>
#endif

#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
#include <DHT.h>
#endif

struct State {
    bool connected = false;
    bool provisioned = false;
    bool configuration_changed = false;
};

class EnvironmentMonitor {
  public:
    EnvironmentMonitor();
    bool begin();
#if ENVIRONMENTMONITOR_LPM > 0
    void sleep();
#endif
    measurement* data(byte *ptr);
    bool pollSensors();
    bool hasBMP085();
    bool hasBME280();
    bool hasMCP9808();
    bool hasDHT22();

  private:
State m_state;
    void startSensors();
    measurement m_data[ENVIRONMENTMONITOR_HISTORY_LENGTH];
    byte m_data_p=0;
    byte m_pollCnt;
    bool m_ready;
    // fixme: make it easy to enumerate connected sensors
#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
    Adafruit_BMP085 bmp;
#endif
#if ENVIRONMENTMONITOR_SENSOR_BME280 > 0
 #if ENVIRONMENTMONITOR_BME_LIBRARY == L_Adafruit
    Adafruit_BME280 bme;
    Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
    Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
    Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Bosch
    Bme280BoschWrapper bme = Bme280BoschWrapper(true);
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Glenn
    BME280I2C bme;
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_SparkFun
    BME280 bme;
 #elif ENVIRONMENTMONITOR_BME_LIBRARY == L_Forced
    ForcedClimate bme = ForcedClimate();
 #endif
#endif
#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
    Adafruit_MCP9808 mcp9808 = Adafruit_MCP9808();
#endif
#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
    DHT dht22 = DHT(9, DHT22);
#endif
};

#endif
