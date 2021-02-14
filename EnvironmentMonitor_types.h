/**
 * @file EnvironmentMonitor_types.h
 * @copyright 2015 Bernd Wachter
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2015
 */

#ifndef _ENVIRONMENTMONITOR_TYPES_H
#define _ENVIRONMENTMONITOR_TYPES_H

#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
typedef struct {
#if ENVIRONMENTMONITOR_SENSOR_IDENTIFIER >0
    char type = 'b';
#endif
    float temperature = 0;
    uint32_t pressure = 0;
} _bmp085;
#endif

#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
typedef struct {
#if ENVIRONMENTMONITOR_SENSOR_IDENTIFIER >0
    char type = 'm';
#endif
    float temperature = 0;
} _mcp9808;
#endif

#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
typedef struct {
#if ENVIRONMENTMONITOR_SENSOR_IDENTIFIER >0
    char type = 'd';
#endif
    float temperature = 0;
    float humidity = 0;
} _dht22;
#endif

#if ENVIRONMENTMONITOR_SENSOR_RAIN > 0
typedef struct {
#if ENVIRONMENTMONITOR_SENSOR_IDENTIFIER >0
    char type = 'r';
#endif
    uint32_t rain = 0;
} _rain;
#endif

typedef struct {
#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
    _bmp085 bmp085;
#endif
#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
    _mcp9808 mcp9808;
#endif
#if ENVIRONMENTMONITOR_SENSOR_DHT22 > 0
    _dht22 dht22;
#endif
#if ENVIRONMENTMONITOR_SENSOR_RAIN > 0
    _rain rain;
#endif
#if ENVIRONMENT_HISTORY_LENGTH > 1
    int32_t id = 0;  // send id and id of last measurement so that server can detect missing data
    int32_t prev_id = 0;
#endif
} measurement;

struct {
#if ENVIRONMENTMONITOR_SENSOR_BMP085 > 0
    bool bmp085 = false;
#endif
#if ENVIRONMENTMONITOR_SENSOR_MCP9808 > 0
    bool mcp9808 = false;
#endif
    bool dht22 = false;
} sensors;

/*
struct {
    configuration config;
    int measurement_size = sizeof(measurement);
    // sensors
    //measurement m[ENVIRONMENTMONITOR_HISTORY_LENGTH];
} transmission;
*/

#endif
