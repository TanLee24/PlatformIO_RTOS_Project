#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

extern float glob_temperature;
extern float glob_humidity;
extern int glob_ml_state; // For tinyML

extern String AP_SSID;
extern String AP_PASS;
extern String STA_SSID;
extern String STA_PASS;

extern String CORE_IOT_TOKEN;
extern String CORE_IOT_SERVER;
extern String CORE_IOT_PORT;

extern boolean isWifiConnected;

extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern QueueHandle_t xQueueSoilMoisture; // Queue to get the value of soil moisture
extern QueueHandle_t xButtonQueue; // Queue truyền sự kiện nhấn nút

#endif