/**
 * @file EnvironmentMonitor.h
 * @copyright 2015 Bernd Wachter
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2015
 */

#ifndef _ENVIRONMENTMONITOR_H
#define _ENVIRONMENTMONITOR_H

#ifndef ENVIRONMENTMONITOR_SENSOR_BMP085
#define ENVIRONMENTMONITOR_SENSOR_BMP085 0
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_MCP9808
#define ENVIRONMENTMONITOR_SENSOR_MCP9808 0
#endif

#ifndef ENVIRONMENTMONITOR_SENSOR_DHT22
#define ENVIRONMENTMONITOR_SENSOR_DHT22 0
#endif

// the purpose for this is to keep history on unreliable transmission media as
// well as being able to calculate averages
#ifndef ENVIRONMENTMONITOR_HISTORY_LENGTH
#define ENVIRONMENTMONITOR_HISTORY_LENGTH 1
#endif

#include "EnvironmentMonitor_types.h"

#ifndef ENVIRONMENTMONITOR_LPM
#define ENVIRONMENTMONITOR_LPM 0
#endif

#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
#include <Adafruit_BMP085.h>
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
#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
    Adafruit_MCP9808 mcp9808 = Adafruit_MCP9808();
#endif
#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
    DHT dht22 = DHT(9, DHT22);
#endif
};

#endif
