#ifndef __COREIOT_H__
#define __COREIOT_H__

#include <Arduino.h>
#include <WiFi.h>
#include "global.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED_GPIO 48
// void callback(char* topic, byte* payload, unsigned int length);
void coreiot_task(void *pvParameters);

#endif