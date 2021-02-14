/**
 * @file EnvironmentMonitor.cpp
 * @copyright 2015 Bernd Wachter
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2015
 */

#include <Arduino.h>
#include <EEPROM.h>
#include <MACTool.h>

#include "EnvironmentMonitor.h"

#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

ISR(WDT_vect){
}

EnvironmentMonitor::EnvironmentMonitor(){
}

bool EnvironmentMonitor::begin(){
  // TODO: check configuration structure with sensor data
  startSensors();
}

void EnvironmentMonitor::setupWatchdog(){
  noInterrupts();

  // Set the watchdog reset bit in the MCU status register to 0.
  MCUSR &= ~(1<<WDRF);

  // Set WDCE and WDE bits in the watchdog control register.
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  // Set watchdog clock prescaler bits to a value of 8 seconds.
  WDTCSR = (1<<WDP0) | (1<<WDP3);

  // Enable watchdog as interrupt only (no reset).
  WDTCSR |= (1<<WDIE);

  // Enable interrupts again.
  interrupts();
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

char* EnvironmentMonitor::id(){
  // FIXME
  return "";
}

bool EnvironmentMonitor::pollSensors(){
  if (m_pollCnt >= ENVIRONMENTMONITOR_HISTORY_LENGTH && !m_ready)
    m_ready = true;

#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
  if (sensors.bmp085){
    m_data[m_data_p].bmp085.temperature = bmp.readTemperature();
    m_data[m_data_p].bmp085.pressure = bmp.readPressure();
  }
#endif

#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
  if (sensors.mcp9808){
    //mcp9808.shutdown_wake(0);
    m_data[m_data_p].mcp9808.temperature = mcp9808.readTempC();
    //mcp9808.shutdown_wake(1);
  }
#endif

#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
  if (sensors.dht22){
    m_data[m_data_p].dht22.temperature = dht22.readTemperature();
    m_data[m_data_p].dht22.humidity = dht22.readHumidity();
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
#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
  if (mcp9808.begin())
    sensors.mcp9808 = true;
#endif
#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
  dht22.begin();
  sensors.dht22 = true;
#endif
}

bool EnvironmentMonitor::hasBMP085(){
#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
  return sensors.bmp085;
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