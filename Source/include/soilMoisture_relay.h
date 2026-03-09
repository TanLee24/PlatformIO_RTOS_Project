#ifndef __SOILMOISTURE_RELAY_H__
#define __SOILMOISTURE_RELAY_H__

#include <Arduino.h>
#include "global.h"

#define SOIL_SENSOR_PIN GPIO_NUM_2
#define RELAY_PIN GPIO_NUM_8

// Task to monitor soil moisture and control pump (relay)
void soilMoistureAffectsPump(void *pvParameters);

#endif